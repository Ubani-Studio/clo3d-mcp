"""Load your own tools into the server without forking it.

Set CLO3D_MCP_EXTENSIONS to one or more folders or .py files (separated by ':' on macOS and Linux,
';' on Windows). Every .py file whose name does not start with '_' is loaded, and its
register(ext) function is called with an ExtensionContext:

    def register(ext):
        @ext.tool()
        def rename_all(prefix: str) -> dict:
            "Prefix every pattern piece name."
            pieces = ext.send("get_pattern_list")
            ...

ext.send(command, params) talks to the CLO3D plug-in exactly as the built-in tools do, so an
extension composes the commands the plug-in already handles. Your extension files stay wherever
you keep them, private repo included; nothing in them is copied into this package.

A file that fails to load is reported by name, never skipped in silence: the server logs it to
stderr at start-up and the list_extensions tool returns it with the reason. An extension may not
replace a built-in tool or another extension's tool; a clashing name is refused.
"""

from __future__ import annotations

import importlib.util
import os
import sys
import traceback
from typing import Callable

ENV = "CLO3D_MCP_EXTENSIONS"


class ExtensionContext:
    """What an extension's register() receives: a tool decorator that refuses clashes, and send()."""

    def __init__(self, mcp, send: Callable, source: str, taken: set[str]):
        self.mcp = mcp
        self.send = send
        self.source = source
        self._taken = taken
        self.added: list[str] = []

    def tool(self, name: str | None = None, description: str | None = None):
        def decorate(fn):
            tool_name = name or fn.__name__
            if tool_name in self._taken:
                raise ValueError(f"tool name {tool_name!r} is already registered; an extension cannot replace it")
            self.mcp.add_tool(fn, name=tool_name, description=description)
            self._taken.add(tool_name)
            self.added.append(tool_name)
            return fn

        return decorate


def _files(entry: str) -> list[str]:
    if os.path.isdir(entry):
        return [os.path.join(entry, f) for f in sorted(os.listdir(entry))
                if f.endswith(".py") and not f.startswith("_")]
    if os.path.isfile(entry) and entry.endswith(".py"):
        return [entry]
    raise FileNotFoundError(f"{entry!r} is neither a folder nor a .py file")


def _tool_names(mcp) -> set[str]:
    return {t.name for t in mcp._tool_manager.list_tools()}


def load_extensions(mcp, send: Callable, spec: str | None = None) -> dict:
    """Load every extension named by `spec` (default: the CLO3D_MCP_EXTENSIONS variable).

    Returns {"loaded": [{"file", "tools"}], "failed": [{"file", "error"}]}. Never raises: a broken
    extension must not take the built-in tools down with it.
    """
    raw = os.environ.get(ENV, "") if spec is None else spec
    report = {"loaded": [], "failed": []}
    taken = _tool_names(mcp)
    for entry in (e.strip() for e in raw.split(os.pathsep)):
        if not entry:
            continue
        try:
            files = _files(entry)
        except Exception as e:  # a missing folder is a fault worth naming
            report["failed"].append({"file": entry, "error": str(e)})
            continue
        for path in files:
            ctx = ExtensionContext(mcp, send, path, taken)
            try:
                stem = os.path.splitext(os.path.basename(path))[0]
                spec_obj = importlib.util.spec_from_file_location(f"clo3d_mcp_ext_{stem}", path)
                module = importlib.util.module_from_spec(spec_obj)
                spec_obj.loader.exec_module(module)
                register = getattr(module, "register", None)
                if not callable(register):
                    raise AttributeError("defines no register(ext) function")
                register(ctx)
                if not ctx.added:
                    raise ValueError("register(ext) added no tools")
                report["loaded"].append({"file": path, "tools": ctx.added})
            except Exception as e:
                # Undo any tools this file added before it failed, so a half-loaded file is not live.
                for name in ctx.added:
                    if hasattr(mcp._tool_manager, "remove_tool"):
                        mcp._tool_manager.remove_tool(name)
                    else:
                        mcp._tool_manager._tools.pop(name, None)
                    taken.discard(name)
                report["failed"].append({"file": path, "error": f"{type(e).__name__}: {e}"})
                print(f"[clo3d-mcp] extension failed: {path}\n{traceback.format_exc()}", file=sys.stderr)
    for item in report["loaded"]:
        print(f"[clo3d-mcp] extension loaded: {item['file']} ({', '.join(item['tools'])})", file=sys.stderr)
    return report
