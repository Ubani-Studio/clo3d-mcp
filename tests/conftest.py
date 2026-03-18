"""Test fixtures — mock CLO3D socket server for unit testing."""

import json
import socket
import threading
import pytest


class MockCLO3DServer:
    """Minimal mock of the CLO3D plugin socket server."""

    def __init__(self, host="127.0.0.1", port=0):
        self.host = host
        self.port = port
        self._server = None
        self._thread = None
        self._running = False
        self._handlers = {
            "ping": lambda p: {"pong": True, "in_clo3d": False},
            "get_project_info": lambda p: {
                "project_name": "TestProject",
                "project_path": "/tmp/test.zprj",
                "clo_version": "7.0.0",
                "pattern_count": 5,
                "fabric_count": 3,
                "colorway_count": 2,
            },
            "get_pattern_count": lambda p: {"count": 5},
            "get_pattern_list": lambda p: {
                "patterns": [
                    {"index": 0, "name": "Front Bodice"},
                    {"index": 1, "name": "Back Bodice"},
                    {"index": 2, "name": "Sleeve Left"},
                    {"index": 3, "name": "Sleeve Right"},
                    {"index": 4, "name": "Collar"},
                ],
                "count": 5,
            },
            "get_colorways": lambda p: {
                "colorways": [
                    {"index": 0, "name": "Default", "current": True},
                    {"index": 1, "name": "Navy", "current": False},
                ],
                "count": 2,
                "current_index": 0,
            },
            "simulate": lambda p: {"simulated": True, "steps": p.get("steps", 100)},
            "export_obj": lambda p: {
                "exported": True,
                "file_path": p.get("file_path", "/tmp/out.obj"),
                "format": "obj",
            },
        }

    def start(self):
        self._server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self._server.bind((self.host, self.port))
        self.port = self._server.getsockname()[1]  # get assigned port
        self._server.listen(1)
        self._server.settimeout(1.0)
        self._running = True
        self._thread = threading.Thread(target=self._loop, daemon=True)
        self._thread.start()

    def stop(self):
        self._running = False
        if self._thread:
            self._thread.join(timeout=3)
        if self._server:
            self._server.close()

    def _loop(self):
        while self._running:
            try:
                client, _ = self._server.accept()
            except socket.timeout:
                continue
            except OSError:
                break

            client.setblocking(True)
            client.settimeout(5.0)
            buf = b""

            while self._running:
                try:
                    chunk = client.recv(65536)
                    if not chunk:
                        break
                    buf += chunk

                    while b"\n" in buf:
                        line, buf = buf.split(b"\n", 1)
                        if not line.strip():
                            continue
                        request = json.loads(line)
                        req_id = request.get("id")
                        cmd_type = request.get("type")
                        params = request.get("params", {})

                        handler = self._handlers.get(cmd_type)
                        if handler:
                            result = handler(params)
                            resp = {"id": req_id, "status": "success", "result": result}
                        else:
                            resp = {
                                "id": req_id,
                                "status": "error",
                                "message": f"Unknown command: {cmd_type}",
                            }
                        client.sendall((json.dumps(resp) + "\n").encode())
                except (socket.timeout, ConnectionResetError, BrokenPipeError, OSError):
                    break

            try:
                client.close()
            except OSError:
                pass


@pytest.fixture
def mock_server():
    """Start a mock CLO3D server and yield it. Stops on teardown."""
    server = MockCLO3DServer()
    server.start()
    yield server
    server.stop()
