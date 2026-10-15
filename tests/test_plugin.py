"""The plug-in and the server, held to one contract.

Both bugs fixed in v0.1.1 lived between the two halves: commands the server exposed that the plug-in never
handled ("Unknown command"), and parameters the server sent that the plug-in never read. These tests read both
files, so the two halves cannot drift apart again, and run every plug-in handler against a fake CLO3D API so a
handler that crashes (TypeError, KeyError, AttributeError) is caught without CLO3D installed.
"""

import ast
import json
import os
import re
import sys
import types

import pytest

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PLUGIN = os.path.join(ROOT, "plugin", "clo3d_mcp_plugin.py")
SERVER = os.path.join(ROOT, "src", "clo3d_mcp", "server.py")
CLO_MODULES = ("export_api", "fabric_api", "import_api", "pattern_api", "utility_api")


class FakeAPI(types.ModuleType):
    """Any CLO3D call answers with a value of the shape its name implies, and is recorded."""

    def __init__(self, name):
        super().__init__(name)
        self.calls = []

    def __getattr__(self, attr):
        if attr.startswith("__"):
            raise AttributeError(attr)

        def call(*args, **kwargs):
            self.calls.append((attr, args, kwargs))
            low = attr.lower()
            if "count" in low:
                return 2
            if low.endswith("list") or low.endswith("names") or "genders" in low or low.endswith("s") and low.startswith("get"):
                return ["a", "b"]
            if "json" in low or "info" in low:
                return "{}"
            if low.startswith(("get", "is", "has")) and ("name" in low or "path" in low or "version" in low):
                return "x"
            if low.startswith(("is", "has")):
                return True
            if "boundingbox" in low or "bounding" in low:
                return [0.0, 0.0, 10.0, 10.0]
            return True

        return call


def load_plugin():
    """The plug-in module with fake CLO3D APIs and WITHOUT its auto-start (which would begin listening)."""
    fakes = {name: FakeAPI(name) for name in CLO_MODULES}
    sys.modules.update(fakes)
    src = open(PLUGIN, encoding="utf-8").read()
    src = re.sub(r"\nif __name__ == \"__main__\" or IN_CLO3D:\n\s+start\(\)\s*$", "\n", src)
    assert "\n    start()" not in src.split("# Auto-start")[-1], "the auto-start line moved; update load_plugin"
    module = types.ModuleType("clo3d_mcp_plugin")
    module.__file__ = PLUGIN
    exec(compile(src, PLUGIN, "exec"), module.__dict__)
    return module, fakes


def server_calls():
    """Every _send("command", {params}) in server.py, with the literal keys of its params dict."""
    tree = ast.parse(open(SERVER, encoding="utf-8").read())
    out = {}
    for node in ast.walk(tree):
        if isinstance(node, ast.Call) and getattr(node.func, "id", None) == "_send" and node.args:
            cmd = node.args[0]
            if not isinstance(cmd, ast.Constant):
                continue
            keys = set()
            if len(node.args) > 1 and isinstance(node.args[1], ast.Dict):
                keys = {k.value for k in node.args[1].keys if isinstance(k, ast.Constant)}
            out.setdefault(cmd.value, set()).update(keys)
    return out


def handler_reads():
    """For each handler function, the params keys it reads (params["k"] or params.get("k"))."""
    tree = ast.parse(open(PLUGIN, encoding="utf-8").read())
    reads = {}
    for fn in tree.body:
        if isinstance(fn, ast.FunctionDef) and fn.name.startswith("handle_"):
            keys = set()
            for node in ast.walk(fn):
                if isinstance(node, ast.Subscript) and getattr(node.value, "id", None) == "params" and isinstance(node.slice, ast.Constant):
                    keys.add(node.slice.value)
                if isinstance(node, ast.Call) and isinstance(node.func, ast.Attribute) and node.func.attr == "get" \
                        and getattr(node.func.value, "id", None) == "params" and node.args and isinstance(node.args[0], ast.Constant):
                    keys.add(node.args[0].value)
            reads[fn.name] = keys
    return reads


def sample(key):
    k = key.lower()
    if k in ("r", "g", "b", "a"):
        return 128
    if k == "options":
        return {}
    if "points" in k or "vertices" in k:
        return [[0, 0], [100, 0], [100, 100], [0, 100]]
    if "index" in k or k.endswith("_idx") or k in ("steps", "count", "frames", "width", "height", "quality"):
        return 1
    if "color" in k or "colour" in k:
        return [255, 0, 0]
    if "path" in k or "file" in k or "folder" in k or "dir" in k:
        return os.path.join(ROOT, "tests", "_scratch_out", "x.zprj")
    if k.startswith(("is_", "show", "visible", "enable")):
        return True
    return "x"


PLUGIN_MODULE, FAKES = load_plugin()


def test_every_server_command_exists_in_the_plugin():
    unknown = sorted(set(server_calls()) - set(PLUGIN_MODULE.HANDLERS))
    assert not unknown, f"the server sends commands the plug-in does not handle: {unknown}"


def test_every_parameter_the_server_sends_is_read():
    reads = handler_reads()
    dropped = {}
    for cmd, keys in server_calls().items():
        handler = PLUGIN_MODULE.HANDLERS.get(cmd)
        if handler is None:
            continue
        missing = keys - reads.get(handler.__name__, set())
        if missing:
            dropped[cmd] = sorted(missing)
    assert not dropped, f"parameters sent by the server and never read by the plug-in: {dropped}"


@pytest.mark.parametrize("command", sorted(set(PLUGIN_MODULE.HANDLERS) - {"stop"}))
def test_every_handler_runs_against_the_api(command):
    handler = PLUGIN_MODULE.HANDLERS[command]
    params = {k: sample(k) for k in handler_reads().get(handler.__name__, set())}
    reply = json.loads(PLUGIN_MODULE.process_command(json.dumps({"id": "t", "type": command, "params": params})))
    if reply["status"] == "error":
        # a refusal the handler chose to make is fine; a Python crash is a bug in the handler
        assert not re.match(r"(TypeError|KeyError|AttributeError|NameError|IndexError|UnboundLocalError)", reply["message"]), \
            f"{command} crashed: {reply['message']}"


def test_unknown_command_and_bad_json_are_refused_by_name():
    unknown = json.loads(PLUGIN_MODULE.process_command(json.dumps({"id": 1, "type": "no_such_thing"})))
    assert unknown["status"] == "error" and "no_such_thing" in unknown["message"]
    bad = json.loads(PLUGIN_MODULE.process_command("{not json"))
    assert bad["status"] == "error" and "Invalid JSON" in bad["message"]


def test_fabric_count_passes_the_required_bool():
    FAKES["fabric_api"].calls.clear()
    PLUGIN_MODULE.process_command(json.dumps({"id": 1, "type": "get_fabric_count"}))
    calls = [c for c in FAKES["fabric_api"].calls if c[0] == "GetFabricCount"]
    assert calls and calls[0][1] == (True,), f"GetFabricCount must be called with True (both overloads take one argument): {calls}"
