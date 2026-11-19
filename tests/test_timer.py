"""The timer listener is on by default only from the first CLO3D build it was verified on; the environment
variable overrides either way. CLO3D 2024.1 never fires the timer and crashed with it, so anything older, or a
version the API does not give, stays off."""

import types

import pytest

from test_plugin import load_plugin


def plugin_on(major, minor):
    module, _ = load_plugin()
    module.utility_api = types.SimpleNamespace(GetMajorVersion=lambda: major, GetMinorVersion=lambda: minor)
    return module


@pytest.mark.parametrize("version,expected", [((2024, 1), False), ((2025, 1), False), ((2025, 2), True),
                                              ((2026, 0), True), ((7, 3), False)])
def test_default_follows_the_clo_version(monkeypatch, version, expected):
    monkeypatch.delenv("CLO3D_MCP_TIMER", raising=False)
    assert plugin_on(*version)._timer_enabled() is expected


@pytest.mark.parametrize("flag,expected", [("1", True), ("true", True), ("0", False), ("off", False)])
def test_the_variable_overrides_the_version(monkeypatch, flag, expected):
    monkeypatch.setenv("CLO3D_MCP_TIMER", flag)
    assert plugin_on(2024, 1)._timer_enabled() is expected
    assert plugin_on(2025, 2)._timer_enabled() is expected


def test_an_unreadable_version_keeps_the_timer_off(monkeypatch):
    monkeypatch.delenv("CLO3D_MCP_TIMER", raising=False)
    module, _ = load_plugin()            # the fake API answers "x" for a version: not a number

    def boom():
        raise RuntimeError("no version")
    assert module._timer_enabled() is False
    module.utility_api = types.SimpleNamespace(GetMajorVersion=boom, GetMinorVersion=boom)
    assert module._timer_enabled() is False
    assert "not readable" in module._clo_version_text()
