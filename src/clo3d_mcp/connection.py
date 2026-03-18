"""
CLO3D TCP socket connection client.

Manages a persistent socket connection to the CLO3D plugin server.
Sends JSON commands over newline-delimited protocol and parses responses.
"""

import json
import socket
import uuid
import time

DEFAULT_HOST = "127.0.0.1"
DEFAULT_PORT = 9877
TIMEOUT = 180  # seconds — simulation/export can take minutes
MAX_RETRIES = 3
RETRY_DELAY = 1.0


class CLO3DConnectionError(Exception):
    pass


class CLO3DConnection:
    """Singleton TCP client for the CLO3D plugin socket server."""

    _instance = None

    def __new__(cls, host=DEFAULT_HOST, port=DEFAULT_PORT):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
            cls._instance._initialized = False
        return cls._instance

    def __init__(self, host=DEFAULT_HOST, port=DEFAULT_PORT):
        if self._initialized:
            return
        self.host = host
        self.port = port
        self._socket = None
        self._buffer = b""
        self._initialized = True

    @property
    def connected(self):
        return self._socket is not None

    def connect(self):
        """Establish connection to CLO3D plugin."""
        if self._socket:
            return

        try:
            self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self._socket.settimeout(TIMEOUT)
            self._socket.connect((self.host, self.port))
            self._buffer = b""
        except (ConnectionRefusedError, socket.timeout, OSError) as e:
            self._socket = None
            raise CLO3DConnectionError(
                f"Cannot connect to CLO3D on {self.host}:{self.port}. "
                f"Is CLO3D running with the MCP plugin loaded? Error: {e}"
            )

    def disconnect(self):
        """Close the connection."""
        if self._socket:
            try:
                self._socket.close()
            except OSError:
                pass
            self._socket = None
            self._buffer = b""

    def _ensure_connected(self):
        """Connect if not already connected."""
        if not self._socket:
            self.connect()

    def _recv_line(self):
        """Read until we get a newline-terminated JSON response."""
        while b"\n" not in self._buffer:
            try:
                chunk = self._socket.recv(65536)
                if not chunk:
                    self.disconnect()
                    raise CLO3DConnectionError("CLO3D closed the connection")
                self._buffer += chunk
            except socket.timeout:
                raise CLO3DConnectionError(
                    f"Timed out waiting for CLO3D response ({TIMEOUT}s). "
                    "The operation may still be running in CLO3D."
                )
            except (ConnectionResetError, BrokenPipeError, OSError) as e:
                self.disconnect()
                raise CLO3DConnectionError(f"Connection lost: {e}")

        line, self._buffer = self._buffer.split(b"\n", 1)
        return line.decode("utf-8")

    def send_command(self, command_type, params=None, retries=MAX_RETRIES):
        """
        Send a command to CLO3D and return the result.

        Args:
            command_type: The command name (e.g., "get_project_info")
            params: Optional dict of parameters
            retries: Number of reconnection attempts on failure

        Returns:
            The result dict from the response

        Raises:
            CLO3DConnectionError: On connection/communication failure
        """
        request = {
            "id": str(uuid.uuid4()),
            "type": command_type,
            "params": params or {},
        }
        payload = json.dumps(request) + "\n"

        for attempt in range(retries):
            try:
                self._ensure_connected()
                self._socket.sendall(payload.encode("utf-8"))
                response_str = self._recv_line()
                response = json.loads(response_str)

                if response.get("status") == "error":
                    error_msg = response.get("message", "Unknown error from CLO3D")
                    raise CLO3DConnectionError(f"CLO3D error: {error_msg}")

                return response.get("result", {})

            except CLO3DConnectionError:
                if attempt < retries - 1:
                    self.disconnect()
                    time.sleep(RETRY_DELAY)
                    continue
                raise

    def ping(self):
        """Test the connection. Returns True if CLO3D responds."""
        try:
            result = self.send_command("ping", retries=1)
            return result.get("pong", False)
        except CLO3DConnectionError:
            return False


# Module-level singleton
_connection = None


def get_connection(host=DEFAULT_HOST, port=DEFAULT_PORT):
    """Get or create the global CLO3D connection."""
    global _connection
    if _connection is None:
        _connection = CLO3DConnection(host, port)
    return _connection
