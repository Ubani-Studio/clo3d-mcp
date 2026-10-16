"""
CLO3D file-based connection client.

Communicates with the CLO3D plugin via a shared directory.
The MCP server writes req_<id>.json, the plugin reads it, processes,
and writes resp_<id>.json. Both sides use atomic writes (temp + rename).
Per-request files let several MCP servers (one per Claude session) share
the directory without reading or overwriting each other's messages.

On WSL, auto-detects the Windows temp directory for the shared path.
Override with CLO3D_MCP_DIR environment variable if needed; the plug-in
honours the same variable, so setting it on the server is enough.
"""

import json
import os
import tempfile
import time
import uuid

TIMEOUT = 180  # seconds: simulation/export can take minutes
PICKUP_TIMEOUT = 5  # seconds for the plugin to pick a request up before we give up
POLL_INTERVAL = 0.05  # seconds between file checks


class CLO3DConnectionError(Exception):
    pass


def _remove(path):
    try:
        os.remove(path)
    except OSError:
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

    # 3. Fallback: the platform temp dir.
    #    TEMP is a Windows variable. On macOS/Linux it is unset, and the two
    #    sides used DIFFERENT fallbacks (server /tmp, plugin ~), so they never
    #    met. tempfile.gettempdir() honours TEMP/TMP/TMPDIR on every platform,
    #    so both sides now agree.
    return os.path.join(tempfile.gettempdir(), "clo3d_mcp")


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

    def send_command(self, command_type, params=None, retries=None):
        """
        Send a command to CLO3D and return the result.

        Writes req_<id>.json, waits for resp_<id>.json, returns the parsed
        result. Commands are never re-sent: a retry could repeat a command
        that changes the scene, such as create_pattern.

        `retries` is accepted for backward compatibility and ignored.
        """
        request = {
            "id": str(uuid.uuid4()),
            "type": command_type,
            "params": params or {},
        }
        return self._do_send(request)

    def _do_send(self, request):
        """Write request, wait for the plugin to pick it up and answer it."""
        if not os.path.isdir(self.comm_dir):
            raise CLO3DConnectionError(
                "CLO3D communication directory not found: " + self.comm_dir + ". "
                "Is CLO3D running with the MCP plugin loaded?"
            )

        req_id = request["id"]
        request_file = os.path.join(self.comm_dir, "req_" + req_id + ".json")
        response_file = os.path.join(self.comm_dir, "resp_" + req_id + ".json")

        # Write request atomically. The .tmp suffix keeps the plugin from
        # matching the file until it is complete.
        tmp_file = request_file + ".tmp"
        with open(tmp_file, "w") as f:
            f.write(json.dumps(request))
        os.rename(tmp_file, request_file)

        start_time = time.time()
        while True:
            elapsed = time.time() - start_time

            if os.path.exists(response_file):
                try:
                    with open(response_file, "r") as f:
                        response = json.loads(f.read())
                except (OSError, json.JSONDecodeError, ValueError):
                    time.sleep(POLL_INTERVAL)
                    continue
                _remove(response_file)

                if response.get("status") == "error":
                    error_msg = response.get("message", "Unknown error from CLO3D")
                    raise CLO3DConnectionError("CLO3D error: " + error_msg)
                return response.get("result", {})

            if os.path.exists(request_file):
                if elapsed > PICKUP_TIMEOUT:
                    # Withdraw the request so the plugin can't run it later.
                    _remove(request_file)
                    raise CLO3DConnectionError(
                        "CLO3D plugin is not responding (request not picked up within "
                        + str(PICKUP_TIMEOUT) + "s). Check that CLO3D is open with the "
                        "native plugin installed, and open Script > Script Editor once "
                        "after launching CLO3D: CLO3D only starts Python when the "
                        "editor is first opened."
                    )
            elif elapsed > TIMEOUT:
                raise CLO3DConnectionError(
                    "Timed out waiting for CLO3D response (" + str(TIMEOUT) + "s). "
                    "The operation may still be running in CLO3D."
                )

            time.sleep(POLL_INTERVAL)

    def ping(self):
        """Test the connection. Returns True if CLO3D responds."""
        try:
            result = self.send_command("ping")
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
