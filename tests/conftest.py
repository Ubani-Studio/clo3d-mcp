"""Test fixtures: mock CLO3D file-based server for unit testing."""

import json
import os
import tempfile
import threading
import time
import pytest


class MockCLO3DServer:
    """Minimal mock of the CLO3D plugin file-based server.

    Polls for req_<id>.json files in a temp directory, processes commands
    using built-in handlers, and writes resp_<id>.json.
    """

    def __init__(self):
        self.comm_dir = tempfile.mkdtemp(prefix="clo3d_mcp_test_")
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
        self._running = True
        self._thread = threading.Thread(target=self._loop, daemon=True)
        self._thread.start()

    def stop(self):
        self._running = False
        if self._thread:
            self._thread.join(timeout=3)
        # Clean up temp dir
        for name in os.listdir(self.comm_dir):
            try:
                os.remove(os.path.join(self.comm_dir, name))
            except OSError:
                pass
        try:
            os.rmdir(self.comm_dir)
        except OSError:
            pass

    def _loop(self):
        while self._running:
            for name in sorted(os.listdir(self.comm_dir)):
                if not (name.startswith("req_") and name.endswith(".json")):
                    continue
                try:
                    self._handle(os.path.join(self.comm_dir, name))
                except Exception as e:
                    print(f"[MockServer] Error: {e}")
            time.sleep(0.02)

    def _handle(self, request_file):
        with open(request_file, "r") as f:
            request = json.loads(f.read())
        os.remove(request_file)

        req_id = request.get("id")
        cmd_type = request.get("type")
        handler = self._handlers.get(cmd_type)
        if handler:
            resp = {"id": req_id, "status": "success", "result": handler(request.get("params", {}))}
        else:
            resp = {"id": req_id, "status": "error", "message": f"Unknown command: {cmd_type}"}

        response_file = os.path.join(self.comm_dir, f"resp_{req_id}.json")
        tmp = response_file + ".tmp"
        with open(tmp, "w") as f:
            f.write(json.dumps(resp))
        os.rename(tmp, response_file)


@pytest.fixture
def mock_server():
    """Start a mock CLO3D file server and yield it. Stops on teardown."""
    server = MockCLO3DServer()
    server.start()
    yield server
    server.stop()
