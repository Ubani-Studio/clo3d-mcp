"""Automatic layout of the pieces on the fabric width."""

from clo3d_mcp.server import _send, mcp


@mcp.tool()
def start_nesting(buffer_spacing: float | None = None, target_colorways: list[int] | None = None) -> dict:
    """Start the automatic layout of the pieces on the fabric width, to measure fabric consumption.

    Args:
        buffer_spacing: Optional gap between pieces in mm.
        target_colorways: Optional colorways that receive the layout.
    """
    return _send("start_nesting", {"buffer_spacing": buffer_spacing, "target_colorways": target_colorways})


@mcp.tool()
def stop_nesting() -> dict:
    """Stop the automatic layout."""
    return _send("stop_nesting")


@mcp.tool()
def get_nesting_status() -> dict:
    """Get the layout duration, the gap between pieces and the target colorway."""
    return _send("get_nesting_status")


@mcp.tool()
def set_nesting_pattern(pattern_index: int, x: int | None = None, y: int | None = None,
                        grain_mode: int | None = None) -> dict:
    """Fix a piece at (x, y) in the layout, or set how it may turn: 0 one way, 1 two ways, 2 four ways."""
    return _send("set_nesting_pattern", {"pattern_index": pattern_index, "x": x, "y": y, "grain_mode": grain_mode})
