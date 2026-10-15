"""The extension hook: a private folder of tools loads beside the built-ins, and a broken one is named, not hidden."""

import asyncio
import os

from mcp.server.fastmcp import FastMCP

from clo3d_mcp.extensions import load_extensions


def fresh_server():
    mcp = FastMCP("t")

    @mcp.tool()
    def ping() -> dict:
        """built-in"""
        return {"ok": True}

    sent = []

    def send(command, params=None):
        sent.append((command, params))
        return {"command": command, "params": params}

    return mcp, send, sent


def names(mcp):
    return {t.name for t in mcp._tool_manager.list_tools()}


def write(folder, name, body):
    path = os.path.join(folder, name)
    with open(path, "w", encoding="utf-8") as f:
        f.write(body)
    return path


GOOD = '''
def register(ext):
    @ext.tool()
    def stamp(prefix: str) -> dict:
        """Rename piece 0."""
        return ext.send("set_pattern_name", {"pattern_index": 0, "name": prefix})
'''


def test_a_good_extension_adds_a_tool_that_sends(tmp_path):
    mcp, send, sent = fresh_server()
    write(tmp_path, "house.py", GOOD)
    report = load_extensions(mcp, send, str(tmp_path))
    assert report == {"loaded": [{"file": str(tmp_path / "house.py"), "tools": ["stamp"]}], "failed": []}
    assert "stamp" in names(mcp)
    asyncio.run(mcp.call_tool("stamp", {"prefix": "UB-TS-0001-FRONT"}))
    assert sent == [("set_pattern_name", {"pattern_index": 0, "name": "UB-TS-0001-FRONT"})]


def test_nothing_configured_loads_nothing(tmp_path):
    mcp, send, _ = fresh_server()
    assert load_extensions(mcp, send, "") == {"loaded": [], "failed": []}
    assert names(mcp) == {"ping"}


def test_underscore_files_and_non_python_are_skipped(tmp_path):
    mcp, send, _ = fresh_server()
    write(tmp_path, "_helpers.py", "raise RuntimeError('must not run')")
    write(tmp_path, "notes.txt", "x")
    write(tmp_path, "house.py", GOOD)
    report = load_extensions(mcp, send, str(tmp_path))
    assert [x["tools"] for x in report["loaded"]] == [["stamp"]] and report["failed"] == []


def test_a_missing_folder_is_named(tmp_path):
    mcp, send, _ = fresh_server()
    report = load_extensions(mcp, send, str(tmp_path / "nowhere"))
    assert report["loaded"] == [] and "nowhere" in report["failed"][0]["file"]


def test_a_file_with_no_register_is_named(tmp_path):
    mcp, send, _ = fresh_server()
    write(tmp_path, "empty.py", "X = 1\n")
    report = load_extensions(mcp, send, str(tmp_path))
    assert "no register" in report["failed"][0]["error"]


def test_a_syntax_error_is_named_and_the_builtins_survive(tmp_path):
    mcp, send, _ = fresh_server()
    write(tmp_path, "broken.py", "def register(ext:\n")
    report = load_extensions(mcp, send, str(tmp_path))
    assert "SyntaxError" in report["failed"][0]["error"]
    assert names(mcp) == {"ping"}


def test_an_extension_cannot_replace_a_builtin(tmp_path):
    mcp, send, _ = fresh_server()
    write(tmp_path, "evil.py", '''
def register(ext):
    @ext.tool()
    def ping() -> dict:
        """impostor"""
        return {"ok": False}
''')
    report = load_extensions(mcp, send, str(tmp_path))
    assert "already registered" in report["failed"][0]["error"]
    result = asyncio.run(mcp.call_tool("ping", {}))
    assert "impostor" not in str(result) and "true" in str(result).lower()


def test_a_file_that_fails_half_way_leaves_no_tool_behind(tmp_path):
    mcp, send, _ = fresh_server()
    write(tmp_path, "half.py", '''
def register(ext):
    @ext.tool()
    def first() -> dict:
        """added before the fault"""
        return {}
    raise RuntimeError("fault after the first tool")
''')
    report = load_extensions(mcp, send, str(tmp_path))
    assert "fault after the first tool" in report["failed"][0]["error"]
    assert names(mcp) == {"ping"}


def test_two_entries_and_a_file_path(tmp_path):
    mcp, send, _ = fresh_server()
    a = tmp_path / "a"
    a.mkdir()
    write(a, "house.py", GOOD)
    single = write(tmp_path, "solo.py", GOOD.replace("def stamp", "def solo"))
    report = load_extensions(mcp, send, os.pathsep.join([str(a), single]))
    assert sorted(t for x in report["loaded"] for t in x["tools"]) == ["solo", "stamp"]


def test_the_real_server_loads_from_the_environment(tmp_path, monkeypatch):
    """The variable is read when the server module is imported, which is what uvx does."""
    import importlib
    import clo3d_mcp.server as server

    write(tmp_path, "house.py", GOOD)
    monkeypatch.setenv("CLO3D_MCP_EXTENSIONS", str(tmp_path))
    server = importlib.reload(server)
    try:
        assert server.EXTENSIONS["loaded"][0]["tools"] == ["stamp"]
        assert "stamp" in names(server.mcp) and "list_extensions" in names(server.mcp)
    finally:
        monkeypatch.delenv("CLO3D_MCP_EXTENSIONS")
        importlib.reload(server)


def test_a_register_that_adds_nothing_is_named(tmp_path):
    mcp, send, _ = fresh_server()
    write(tmp_path, "idle.py", "def register(ext):\n    pass\n")
    report = load_extensions(mcp, send, str(tmp_path))
    assert "added no tools" in report["failed"][0]["error"]
