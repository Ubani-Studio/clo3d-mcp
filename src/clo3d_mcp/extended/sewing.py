"""Seams, topstitches, seam tape, shirring and elastic. Lines are numbered in the order of the
points given to create_pattern; points of type 3 (Bezier controls) do not start a line."""

from clo3d_mcp.server import _send, mcp


@mcp.tool()
def sew_lines(pattern_a: int, line_a: int, pattern_b: int, line_b: int,
              direction_a: bool = True, direction_b: bool = True,
              child_a: int | None = None, child_b: int | None = None) -> dict:
    """Sew two lines together: outline to outline, outline to an internal shape, or internal to internal.

    Use child_b to sew an outline line of piece A onto a line of an internal shape of piece B, for example
    a patch sewn on a garment at the exact place of an internal shape.

    Args:
        pattern_a: Zero-based index of the first pattern piece.
        line_a: Line index on the first piece, or on its internal shape child_a.
        pattern_b: Zero-based index of the second pattern piece.
        line_b: Line index on the second piece, or on its internal shape child_b.
        direction_a: Sewing direction along line_a (True: forward).
        direction_b: Sewing direction along line_b. Flip it if the seam comes out twisted.
        child_a: Optional internal shape index on piece A (from create_internal_shape).
        child_b: Optional internal shape index on piece B (from create_internal_shape).
    """
    return _send("sew_lines", {"pattern_a": pattern_a, "line_a": line_a, "pattern_b": pattern_b, "line_b": line_b,
                         "direction_a": direction_a, "direction_b": direction_b,
                         "child_a": child_a, "child_b": child_b})


@mcp.tool()
def get_seam_groups(pattern_index: int | None = None) -> dict:
    """List the seam groups of the scene, and those attached to one pattern piece if given.

    Args:
        pattern_index: Optional zero-based pattern index.
    """
    return _send("get_seam_groups", {"pattern_index": pattern_index})


@mcp.tool()
def set_shirring(pattern_index: int, line_index: int = -1, enabled: bool = True, height: float | None = None,
                 interval: float | None = None, extend: bool | None = None) -> dict:
    """Turn gathers (shirring) on or off along a line of a pattern piece.

    Args:
        pattern_index: Zero-based pattern index.
        line_index: Zero-based line index, -1 for all lines.
        enabled: True to gather.
        height: Optional shirring height in mm.
        interval: Optional shirring interval in mm.
        extend: Optional, extend the shirring.
    """
    return _send("set_shirring", {"pattern_index": pattern_index, "line_index": line_index, "enabled": enabled,
                                  "height": height, "interval": interval, "extend": extend})


@mcp.tool()
def set_elastic(pattern_index: int, line_index: int = -1, enabled: bool = True, strength: float | None = None,
                strength_ratio: int | None = None, total_length: float | None = None,
                segment_length: float | None = None) -> dict:
    """Put an elastic along a line of a pattern piece.

    Args:
        pattern_index: Zero-based pattern index.
        line_index: Zero-based line index, -1 for all lines.
        enabled: True to add the elastic.
        strength: Optional elastic strength.
        strength_ratio: Optional strength ratio in percent.
        total_length: Optional finished elastic length in mm.
        segment_length: Optional elastic segment length in mm.
    """
    return _send("set_elastic", {"pattern_index": pattern_index, "line_index": line_index, "enabled": enabled,
                                 "strength": strength, "strength_ratio": strength_ratio,
                                 "total_length": total_length, "segment_length": segment_length})


@mcp.tool()
def find_seam_group(name: str) -> dict:
    """Find a seam group index by its name."""
    return _send("find_seam_group", {"name": name})


@mcp.tool()
def add_seam_topstitch(seam_group_index: int, start_ratio: float = 0.0, end_ratio: float = 1.0,
                       style_index: int = 0) -> dict:
    """Add a topstitch along a seam, over all or part of its length.

    Args:
        seam_group_index: Seam group, from get_seam_groups.
        start_ratio: Start along the seam, 0 to 1.
        end_ratio: End along the seam, 0 to 1.
        style_index: Topstitch style, from get_topstitches.
    """
    return _send("add_seam_topstitch", {"seam_group_index": seam_group_index, "start_ratio": start_ratio,
                                        "end_ratio": end_ratio, "style_index": style_index})


@mcp.tool()
def import_topstitch_style(file_path: str) -> dict:
    """Import a topstitch style file (absolute path)."""
    return _send("import_topstitch_style", {"file_path": file_path})


@mcp.tool()
def get_topstitches(pattern_index: int | None = None) -> dict:
    """List the topstitch styles, and the topstitches assigned to a pattern piece if an index is given."""
    return _send("get_topstitches", {"pattern_index": pattern_index})


@mcp.tool()
def set_topstitch(pattern_index: int, stitch_index: int, style_index: int | None = None,
                  curved: bool | None = None, curved_length: int | None = None, right_angled: bool | None = None,
                  extend_start: bool | None = None, extend_end: bool | None = None,
                  z_offset: float | None = None) -> dict:
    """Change a topstitch already placed on a pattern piece. Only the given settings change."""
    return _send("set_topstitch", {"pattern_index": pattern_index, "stitch_index": stitch_index,
                                   "style_index": style_index, "curved": curved, "curved_length": curved_length,
                                   "right_angled": right_angled, "extend_start": extend_start,
                                   "extend_end": extend_end, "z_offset": z_offset})


@mcp.tool()
def set_topstitch_style_color(style_index: int, line_index: int = -1, rgba: list[int] | None = None,
                              opacity: int | None = None, colorway_index: int | None = None) -> dict:
    """Set the thread color or opacity of a topstitch style.

    Args:
        style_index: Topstitch style index.
        line_index: Stitch row of the style, -1 for all rows.
        rgba: Optional color, four integers 0-255.
        opacity: Optional opacity value.
        colorway_index: Optional colorway.
    """
    return _send("set_topstitch_style_color", {"style_index": style_index, "line_index": line_index,
                                               "rgba": rgba, "opacity": opacity, "colorway_index": colorway_index})


@mcp.tool()
def get_all_stitches() -> dict:
    """Get the properties of every stitch of the scene."""
    return _send("get_all_stitches")


@mcp.tool()
def set_seam_taping(pattern_index: int, line_index: int = -1, enabled: bool = True,
                    width: float | None = None) -> dict:
    """Put a seam tape along a line of a pattern piece (-1 for all lines), with an optional width in mm."""
    return _send("set_seam_taping", {"pattern_index": pattern_index, "line_index": line_index,
                                     "enabled": enabled, "width": width})


@mcp.tool()
def get_topstitch_style_details(line_index: int = 1, flag: bool = False) -> dict:
    """List the topstitch styles with name, number of rows, distance, width and offset.

    Args:
        line_index: Stitch row of the style, starting at 1.
        flag: False lists every style (verified); True returned none in tests.
    """
    return _send("get_topstitch_style_details", {"line_index": line_index, "flag": flag})


@mcp.tool()
def set_topstitch_style(style_index: int, name: str | None = None, number_of_lines: int | None = None,
                        distance: float | None = None, width: float | None = None, offset_index: int | None = None,
                        line_index: int = 1) -> dict:
    """Change a topstitch style: name, number of rows, distance between rows, thread width, offset.
    line_index is the stitch row, starting at 1."""
    return _send("set_topstitch_style", {"style_index": style_index, "name": name,
                                         "number_of_lines": number_of_lines, "distance": distance,
                                         "width": width, "offset_index": offset_index, "line_index": line_index})
