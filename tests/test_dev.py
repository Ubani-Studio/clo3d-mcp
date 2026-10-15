"""Dev mode: test code runs inside CLO3D and the plug-in reloads itself, with no Script Editor. Off by default,
because run_script executes whatever arrives in the request directory."""

import json

from test_plugin import load_plugin


def call(module, command, params):
    return json.loads(module.process_command(json.dumps({"id": "1", "type": command, "params": params})))


def test_dev_commands_do_not_exist_unless_dev_mode_is_on():
    module, _ = load_plugin()
    for cmd in ("run_script", "reload_plugin"):
        r = call(module, cmd, {"code": "print(1)"})
        assert r["status"] == "error" and "Unknown command" in r["message"]


def test_run_script_returns_what_the_code_printed_and_reaches_the_api():
    module, fakes = load_plugin(dev=True)
    r = call(module, "run_script", {"code": "print('n', pattern_api.GetPatternCount())\nresult = 7"})
    assert r["status"] == "success" and r["result"]["ok"]
    assert r["result"]["output"] == "n 2\n" and r["result"]["result"] == "7"
    assert ("GetPatternCount", (), {}) in fakes["pattern_api"].calls


def test_run_script_reports_an_error_with_its_traceback_and_keeps_listening():
    module, _ = load_plugin(dev=True)
    r = call(module, "run_script", {"code": "print('before')\n1/0"})
    assert r["status"] == "success" and not r["result"]["ok"]
    assert "ZeroDivisionError" in r["result"]["error"] and r["result"]["output"] == "before\n"
    assert call(module, "ping", {})["result"]["dev"] is True


def test_reload_refuses_a_broken_file_before_touching_the_running_listener(tmp_path):
    module, _ = load_plugin(dev=True)
    bad = tmp_path / "plugin.py"
    bad.write_text("def broken(:\n")
    r = call(module, "reload_plugin", {"path": str(bad)})
    assert r["status"] == "error" and ("SyntaxError" in r["message"] or "invalid syntax" in r["message"])


def test_reload_runs_the_new_file(tmp_path, monkeypatch):
    import builtins
    module, _ = load_plugin(dev=True)
    new = tmp_path / "plugin.py"
    new.write_text("import builtins\nbuiltins._clo3d_reload_probe = CLO3D_MCP_DEV\n")
    r = call(module, "reload_plugin", {"path": str(new)})
    assert r["status"] == "success" and builtins._clo3d_reload_probe is True
    del builtins._clo3d_reload_probe
