"""Unit tests for CLO3D file-based connection client."""

import os
import threading
import time

import pytest

from clo3d_mcp import connection
from clo3d_mcp.connection import CLO3DConnection, CLO3DConnectionError


class TestCLO3DConnection:
    def _make_conn(self, mock_server):
        """Create a fresh connection pointing at the mock server's temp dir."""
        conn = CLO3DConnection.__new__(CLO3DConnection)
        conn.comm_dir = mock_server.comm_dir
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

    def test_concurrent_senders_get_their_own_responses(self, mock_server):
        # Several MCP servers share one directory; none may steal another's reply.
        conn = self._make_conn(mock_server)
        commands = ["ping", "get_pattern_count", "get_project_info", "get_colorways"] * 5
        results, errors = {}, []

        def send(i, cmd):
            try:
                results[i] = (cmd, conn.send_command(cmd))
            except Exception as e:
                errors.append(e)

        threads = [threading.Thread(target=send, args=(i, c)) for i, c in enumerate(commands)]
        for t in threads:
            t.start()
        for t in threads:
            t.join(timeout=10)

        assert not errors
        assert len(results) == len(commands)
        for cmd, result in results.values():
            if cmd == "get_pattern_count":
                assert result == {"count": 5}
            elif cmd == "ping":
                assert result["pong"] is True

    def test_unanswered_request_fails_fast_and_is_withdrawn(self, tmp_path, monkeypatch):
        # Directory exists but no plugin is polling it.
        monkeypatch.setattr(connection, "PICKUP_TIMEOUT", 0.3)
        conn = CLO3DConnection.__new__(CLO3DConnection)
        conn.comm_dir = str(tmp_path)
        conn._initialized = True

        start = time.time()
        with pytest.raises(CLO3DConnectionError, match="not responding"):
            conn.send_command("get_project_info")
        assert time.time() - start < 2
        assert os.listdir(tmp_path) == []
