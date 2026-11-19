"""Plug-in and server held to one contract (extended): every MCP tool sends the command of its own name, every
plug-in command has a tool (aliases excepted), the plug-in stays ASCII, no tool name is duplicated."""
import ast
import glob
import os
import re


def _errors():
    ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    PLUGIN = os.path.join(ROOT, "plugin", "clo3d_mcp_plugin.py")
    SERVER = [os.path.join(ROOT, "src", "clo3d_mcp", "server.py")] + sorted(
        glob.glob(os.path.join(ROOT, "src", "clo3d_mcp", "extended", "*.py")))
    NO_TOOL = {"ping", "stop"}  # listener control only

    errors = []


    def read(path):
        with open(path, encoding="utf-8") as f:
            return f.read()


    plugin = read(PLUGIN)
    defined = {n.name for n in ast.parse(plugin).body if isinstance(n, ast.FunctionDef)}
    registry = dict(re.findall(r'^\s+"(\w+)": (handle_\w+),', plugin, re.M))
    for command, handler in registry.items():
        if handler not in defined:
            errors.append(f"plugin: command {command} points to undefined {handler}")
    for number, line in enumerate(plugin.splitlines(), 1):
        if any(ord(c) > 127 for c in line):
            errors.append(f"plugin: non-ASCII character on line {number}")

    sent, tools = {}, {}
    for path in SERVER:
        text = read(path)
        short = os.path.relpath(path, ROOT)
        for node in ast.parse(text).body:
            if isinstance(node, ast.FunctionDef) and any("tool" in ast.unparse(d) for d in node.decorator_list):
                if node.name in tools:
                    errors.append(f"duplicate tool: {node.name} ({tools[node.name]} and {short})")
                tools[node.name] = short
        for command in re.findall(r'_send\(\s*"(\w+)"', text):
            sent.setdefault(command, short)
        for node in ast.parse(text).body:  # a tool sends the command of its own name, so plugin and MCP agree
            if isinstance(node, ast.FunctionDef) and any("tool" in ast.unparse(d) for d in node.decorator_list):
                own = re.findall(r'_send\(\s*"(\w+)"', ast.unparse(node))
                if own and node.name not in own:
                    errors.append(f"{short}: tool {node.name} sends {own[0]}: give the plugin command the tool's name")

    for command, where in sorted(sent.items()):
        if command not in registry:
            errors.append(f"server: {where} sends {command}, unknown to the plugin")
    aliases = set(re.findall(r'"(\w+)": "\w+"', plugin[plugin.find('ALIASES = {'):]))
    for command in sorted(set(registry) - set(sent) - NO_TOOL - aliases):
        errors.append(f"plugin: command {command} has no MCP tool")


    return errors, registry, tools


def test_tools_and_commands_agree():
    errors, registry, tools = _errors()
    assert errors == [], "\n".join(errors)
    assert len(tools) >= 170
