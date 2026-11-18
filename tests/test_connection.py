"""Unit tests for CLO3D file-based connection client."""

import os
import pytest
from clo3d_mcp.connection import CLO3DConnection, CLO3DConnectionError


class TestCLO3DConnection:
    def _make_conn(self, mock_server):
        """Create a fresh connection pointing at the mock server's temp dir."""
        conn = CLO3DConnection.__new__(CLO3DConnection)
        conn.comm_dir = mock_server.comm_dir
        conn.request_file = mock_server.request_file
        conn.response_file = mock_server.response_file
        conn._initialized = True
        return conn

    def test_ping(self, mock_server):
        conn = self._make_conn(mock_server)
        assert conn.ping() is True

    def test_get_project_info(self, mock_server):
        conn = self._make_conn(mock_server)
        result = conn.send_command("get_project_info")
        assert result["project_name"] == "TestProject"
        assert result["pattern_count"] == 5

    def test_get_pattern_list(self, mock_server):
        conn = self._make_conn(mock_server)
        result = conn.send_command("get_pattern_list")
        assert result["count"] == 5
        assert len(result["patterns"]) == 5
        assert result["patterns"][0]["name"] == "Front Bodice"

    def test_simulate(self, mock_server):
        conn = self._make_conn(mock_server)
        result = conn.send_command("simulate", {"steps": 50})
        assert result["simulated"] is True
        assert result["steps"] == 50

    def test_export_obj(self, mock_server):
        conn = self._make_conn(mock_server)
        result = conn.send_command("export_obj", {"file_path": "/tmp/test.obj"})
        assert result["exported"] is True
        assert result["format"] == "obj"

    def test_get_colorways(self, mock_server):
        conn = self._make_conn(mock_server)
        result = conn.send_command("get_colorways")
        assert result["count"] == 2
        assert result["colorways"][0]["name"] == "Default"

    def test_unknown_command_raises(self, mock_server):
        conn = self._make_conn(mock_server)
        with pytest.raises(CLO3DConnectionError, match="Unknown command"):
            conn.send_command("nonexistent_command")

    def test_no_comm_dir_ping_fails(self, tmp_path):
        conn = CLO3DConnection.__new__(CLO3DConnection)
        conn.comm_dir = str(tmp_path / "nonexistent")
        conn.request_file = str(tmp_path / "nonexistent" / "request.json")
        conn.response_file = str(tmp_path / "nonexistent" / "response.json")
        conn._initialized = True
        assert conn.ping() is False

    def test_multiple_commands(self, mock_server):
        conn = self._make_conn(mock_server)
        r1 = conn.send_command("ping")
        r2 = conn.send_command("get_pattern_count")
        r3 = conn.send_command("get_project_info")
        assert r1["pong"] is True
        assert r2["count"] == 5
        assert r3["clo_version"] == "7.0.0"


def test_a_command_clo3d_took_is_never_resent(mock_server, monkeypatch):
    """A slow command (long simulation, or a dialog waiting in CLO3D) outlives the timeout. The plug-in has
    already taken request.json, so the client must report it as busy and NOT send it again: resent, it would run
    two or three more times once CLO3D is free (seen with a slow import, 2026-11)."""
    import time
    from clo3d_mcp import connection
    from clo3d_mcp.connection import CLO3DBusyError

    runs = []

    def slow(params):
        runs.append(time.time())
        time.sleep(1.0)
        return {"done": True}

    mock_server._handlers["slow"] = slow
    monkeypatch.setattr(connection, "TIMEOUT", 0.3)
    monkeypatch.setattr(connection, "RETRY_DELAY", 0.05)
    conn = CLO3DConnection.__new__(CLO3DConnection)
    conn.comm_dir, conn.request_file, conn.response_file = (
        mock_server.comm_dir, mock_server.request_file, mock_server.response_file)
    conn._initialized = True
    with pytest.raises(CLO3DBusyError):
        conn.send_command("slow")
    time.sleep(1.5)     # let the mock plug-in run any resent copies
    assert len(runs) == 1, f"the command ran {len(runs)} times"


def test_timeout_can_be_raised_from_the_environment(monkeypatch):
    import importlib
    from clo3d_mcp import connection
    monkeypatch.setenv("CLO3D_MCP_TIMEOUT", "900")
    try:
        assert importlib.reload(connection).TIMEOUT == 900
    finally:
        monkeypatch.delenv("CLO3D_MCP_TIMEOUT")
        importlib.reload(connection)
