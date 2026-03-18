"""Unit tests for CLO3D connection client."""

import pytest
from clo3d_mcp.connection import CLO3DConnection, CLO3DConnectionError


class TestCLO3DConnection:
    def _make_conn(self, mock_server):
        """Create a fresh connection to the mock server."""
        conn = CLO3DConnection.__new__(CLO3DConnection)
        conn.host = mock_server.host
        conn.port = mock_server.port
        conn._socket = None
        conn._buffer = b""
        conn._initialized = True
        return conn

    def test_ping(self, mock_server):
        conn = self._make_conn(mock_server)
        assert conn.ping() is True
        conn.disconnect()

    def test_get_project_info(self, mock_server):
        conn = self._make_conn(mock_server)
        result = conn.send_command("get_project_info")
        assert result["project_name"] == "TestProject"
        assert result["pattern_count"] == 5
        conn.disconnect()

    def test_get_pattern_list(self, mock_server):
        conn = self._make_conn(mock_server)
        result = conn.send_command("get_pattern_list")
        assert result["count"] == 5
        assert len(result["patterns"]) == 5
        assert result["patterns"][0]["name"] == "Front Bodice"
        conn.disconnect()

    def test_simulate(self, mock_server):
        conn = self._make_conn(mock_server)
        result = conn.send_command("simulate", {"steps": 50})
        assert result["simulated"] is True
        assert result["steps"] == 50
        conn.disconnect()

    def test_export_obj(self, mock_server):
        conn = self._make_conn(mock_server)
        result = conn.send_command("export_obj", {"file_path": "/tmp/test.obj"})
        assert result["exported"] is True
        assert result["format"] == "obj"
        conn.disconnect()

    def test_get_colorways(self, mock_server):
        conn = self._make_conn(mock_server)
        result = conn.send_command("get_colorways")
        assert result["count"] == 2
        assert result["colorways"][0]["name"] == "Default"
        conn.disconnect()

    def test_unknown_command_raises(self, mock_server):
        conn = self._make_conn(mock_server)
        with pytest.raises(CLO3DConnectionError, match="Unknown command"):
            conn.send_command("nonexistent_command")
        conn.disconnect()

    def test_connection_refused(self):
        conn = CLO3DConnection.__new__(CLO3DConnection)
        conn.host = "127.0.0.1"
        conn.port = 19999  # nothing listening
        conn._socket = None
        conn._buffer = b""
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
        conn.disconnect()
