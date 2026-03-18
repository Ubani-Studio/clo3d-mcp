"""
CLO3D file-based connection client.

Communicates with the CLO3D plugin via a shared directory.
The MCP server writes request.json, the plugin reads it, processes,
and writes response.json. Both sides use atomic writes (temp + rename).

On WSL, auto-detects the Windows temp directory for the shared path.
Override with CLO3D_MCP_DIR environment variable if needed.
"""

import json
import os
import time
import uuid

TIMEOUT = 180  # seconds: simulation/export can take minutes
POLL_INTERVAL = 0.05  # seconds between file checks
MAX_RETRIES = 3
RETRY_DELAY = 1.0


class CLO3DConnectionError(Exception):
    pass


def _find_comm_dir():
    """Determine the shared communication directory.

    Priority:
    1. CLO3D_MCP_DIR env var (explicit override)
    2. Windows %TEMP%/clo3d_mcp via WSL mount (auto-detect)
    3. System temp dir fallback
    """
    # 1. Explicit override
    env_dir = os.environ.get("CLO3D_MCP_DIR")
    if env_dir:
        return env_dir

    # 2. Auto-detect WSL: look for Windows user temp via /mnt/c
    # CLO3D runs on Windows, so the plugin writes to Windows %TEMP%
    if os.path.isdir("/mnt/c/Users"):
        # Try to find the Windows user from /mnt/c/Users
        try:
            users = [
                d
                for d in os.listdir("/mnt/c/Users")
                if d not in ("Public", "Default", "Default User", "All Users")
                and os.path.isdir(os.path.join("/mnt/c/Users", d))
            ]
            for user in users:
                temp_dir = os.path.join(
                    "/mnt/c/Users", user, "AppData", "Local", "Temp", "clo3d_mcp"
                )
                # If the dir already exists (plugin is running), use it
                if os.path.isdir(temp_dir):
                    return temp_dir
            # If none found yet, use the first real user
            if users:
                return os.path.join(
                    "/mnt/c/Users", users[0], "AppData", "Local", "Temp", "clo3d_mcp"
                )
        except OSError:
            pass

    # 3. Fallback: local temp
    return os.path.join(os.environ.get("TEMP", "/tmp"), "clo3d_mcp")


class CLO3DConnection:
    """File-based IPC client for the CLO3D plugin."""

    _instance = None

    def __new__(cls, comm_dir=None):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
            cls._instance._initialized = False
        return cls._instance

    def __init__(self, comm_dir=None):
        if self._initialized:
            return
        self.comm_dir = comm_dir or _find_comm_dir()
        self.request_file = os.path.join(self.comm_dir, "request.json")
        self.response_file = os.path.join(self.comm_dir, "response.json")
        self._initialized = True

    @property
    def connected(self):
        """Check if the communication directory exists (plugin is likely running)."""
        return os.path.isdir(self.comm_dir)

    def connect(self):
        """Ensure the communication directory exists."""
        if not os.path.isdir(self.comm_dir):
            raise CLO3DConnectionError(
                "Cannot find CLO3D communication directory at: " + self.comm_dir + ". "
                "Is CLO3D running with the MCP plugin loaded?"
            )

    def disconnect(self):
        """No-op for file-based connection (kept for API compatibility)."""
        pass

    def send_command(self, command_type, params=None, retries=MAX_RETRIES):
        """
        Send a command to CLO3D and return the result.

        Writes request.json, waits for response.json, returns the parsed result.
        """
        request = {
            "id": str(uuid.uuid4()),
            "type": command_type,
            "params": params or {},
        }

        for attempt in range(retries):
            try:
                return self._do_send(request)
            except CLO3DConnectionError:
                if attempt < retries - 1:
                    time.sleep(RETRY_DELAY)
                    continue
                raise

    def _do_send(self, request):
        """Write request, poll for response, return result."""
        # Ensure comm dir exists
        if not os.path.isdir(self.comm_dir):
            raise CLO3DConnectionError(
                "CLO3D communication directory not found: " + self.comm_dir + ". "
                "Is CLO3D running with the MCP plugin loaded?"
            )

        # Clean up any stale response file
        if os.path.exists(self.response_file):
            try:
                os.remove(self.response_file)
            except OSError:
                pass

        # Write request atomically
        payload = json.dumps(request)
        tmp_file = self.request_file + ".tmp"
        with open(tmp_file, "w") as f:
            f.write(payload)

        # Atomic rename
        if os.path.exists(self.request_file):
            os.remove(self.request_file)
        os.rename(tmp_file, self.request_file)

        # Poll for response
        start_time = time.time()
        while True:
            elapsed = time.time() - start_time
            if elapsed > TIMEOUT:
                raise CLO3DConnectionError(
                    "Timed out waiting for CLO3D response (" + str(TIMEOUT) + "s). "
                    "The operation may still be running in CLO3D."
                )

            if os.path.exists(self.response_file):
                try:
                    with open(self.response_file, "r") as f:
                        data = f.read()

                    # Delete response file
                    try:
                        os.remove(self.response_file)
                    except OSError:
                        pass

                    if not data.strip():
                        time.sleep(POLL_INTERVAL)
                        continue

                    response = json.loads(data)

                    # Verify this response matches our request
                    if response.get("id") != request["id"]:
                        # Stale response from a previous request, keep waiting
                        time.sleep(POLL_INTERVAL)
                        continue

                    if response.get("status") == "error":
                        error_msg = response.get("message", "Unknown error from CLO3D")
                        raise CLO3DConnectionError("CLO3D error: " + error_msg)

                    return response.get("result", {})

                except (json.JSONDecodeError, ValueError):
                    # File might be partially written, wait and retry
                    time.sleep(POLL_INTERVAL)
                    continue

            time.sleep(POLL_INTERVAL)

    def ping(self):
        """Test the connection. Returns True if CLO3D responds."""
        try:
            result = self.send_command("ping", retries=1)
            return result.get("pong", False)
        except CLO3DConnectionError:
            return False


# Module-level singleton
_connection = None


def get_connection(comm_dir=None):
    """Get or create the global CLO3D connection."""
    global _connection
    if _connection is None:
        _connection = CLO3DConnection(comm_dir)
    return _connection
