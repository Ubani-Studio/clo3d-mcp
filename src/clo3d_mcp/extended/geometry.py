"""Pattern pieces, points, lines, internal shapes and annotations."""

from clo3d_mcp.server import _send, mcp


@mcp.tool()
def get_line_lengths(pattern_index: int, count: int) -> dict:
    """Get the length in mm of the first `count` outline lines of a pattern piece.

    Use it to check which line index matches which edge before sewing.

    Args:
        pattern_index: Zero-based index of the pattern piece.
        count: Number of lines to measure, usually the number of edges of the piece.
    """
    return _send("get_line_lengths", {"pattern_index": pattern_index, "count": count})


@mcp.tool()
def create_internal_shape(pattern_index: int, points: list[list[float]], closed: bool = False,
                          base: bool = False) -> dict:
    """Draw a shape inside a pattern piece: dart lines, placement marks, topstitch guides.

    Args:
        pattern_index: Zero-based index of the parent pattern piece.
        points: [x, y] or [x, y, type] in mm, in the piece's coordinates.
                Type: 0 = straight point, 2 = spline point, 3 = Bézier control point.
        closed: True for a closed shape.
        base: True to create a base line instead of an internal line.
    """
    return _send("create_internal_shape", {"pattern_index": pattern_index, "points": points,
                                           "closed": closed, "base": base})


@mcp.tool()
def convert_shape(pattern_index: int, child_index: int, to: str = "internal") -> dict:
    """Convert a shape of a pattern piece to an internal line or a base line.

    Args:
        pattern_index: Zero-based pattern index.
        child_index: Index of the shape inside the piece.
        to: "internal" or "base".
    """
    return _send("convert_shape", {"pattern_index": pattern_index, "child_index": child_index, "to": to})


@mcp.tool()
def offset_as_internal_line(pattern_index: int, line_index: int, distance: float, number: int = 1,
                            reverse: bool = False, extend: bool = False) -> dict:
    """Create internal lines parallel to an outline line, for topstitching or facings.

    Args:
        pattern_index: Zero-based pattern index.
        line_index: Zero-based outline line index.
        distance: Offset distance in mm.
        number: Number of parallel lines.
        reverse: Offset to the other side.
        extend: Extend the lines to the neighbouring edges.
    """
    return _send("offset_as_internal_line", {"pattern_index": pattern_index, "line_index": line_index,
                                             "distance": distance, "number": number,
                                             "reverse": reverse, "extend": extend})


@mcp.tool()
def delete_line(pattern_index: int, line_index: int) -> dict:
    """Delete a line of a pattern piece."""
    return _send("delete_line", {"pattern_index": pattern_index, "line_index": line_index})


@mcp.tool()
def delete_point(pattern_index: int, point_index: int) -> dict:
    """Delete a point of a pattern piece."""
    return _send("delete_point", {"pattern_index": pattern_index, "point_index": point_index})


@mcp.tool()
def get_pattern_details(pattern_index: int) -> dict:
    """Get the 2D position, area and grain direction of a pattern piece."""
    return _send("get_pattern_details", {"pattern_index": pattern_index})


@mcp.tool()
def set_pattern_position(pattern_index: int, x: float, y: float) -> dict:
    """Move a pattern piece in the 2D window to (x, y), in mm."""
    return _send("set_pattern_position", {"pattern_index": pattern_index, "x": x, "y": y})


@mcp.tool()
def set_grain_direction(pattern_index: int, degrees: float) -> dict:
    """Set the grain line direction of a pattern piece, in degrees (0 = vertical grain)."""
    return _send("set_grain_direction", {"pattern_index": pattern_index, "degrees": degrees})


@mcp.tool()
def unfold_pattern(pattern_index: int, line_index: int, half_symmetry: bool = False) -> dict:
    """Unfold a half pattern piece along one of its lines, as for a piece cut on the fold.

    Args:
        pattern_index: Zero-based pattern index.
        line_index: Line used as the fold.
        half_symmetry: Keep the two halves linked so edits are mirrored.
    """
    return _send("unfold_pattern", {"pattern_index": pattern_index, "line_index": line_index,
                                    "half_symmetry": half_symmetry})


@mcp.tool()
def copy_pattern_with_offset(pattern_index: int, dx: float, dy: float) -> dict:
    """Copy a pattern piece, shifted by (dx, dy) mm in the 2D window. Returns the new pattern index."""
    return _send("copy_pattern_with_offset", {"pattern_index": pattern_index, "dx": dx, "dy": dy})


@mcp.tool()
def move_pattern_relative(pattern_index: int, dx: float, dy: float) -> dict:
    """Move a pattern piece by (dx, dy) mm from its current position in the 2D window."""
    return _send("move_pattern_relative", {"pattern_index": pattern_index, "dx": dx, "dy": dy})


@mcp.tool()
def find_pattern(name: str | None = None, x: float | None = None, y: float | None = None,
                 view: str = "2d") -> dict:
    """Find a pattern index by its name, or by a scene position in the 2D or 3D window.

    Args:
        name: Pattern name. If given, x and y are ignored.
        x: Scene position x.
        y: Scene position y.
        view: "2d" or "3d", for the scene position.
    """
    return _send("find_pattern", {"name": name, "x": x, "y": y, "view": view})


@mcp.tool()
def get_pattern_input_information(pattern_index: int | None = None) -> dict:
    """Get the input data CLO3D keeps for a pattern piece, or for all pieces if no index is given."""
    return _send("get_pattern_input_information", {"pattern_index": pattern_index})


@mcp.tool()
def set_pattern_category(pattern_index: int, category: int | None = None, classification: str | None = None) -> dict:
    """Set the category number or the classification text of a pattern piece (front, sleeve, collar...)."""
    return _send("set_pattern_category", {"pattern_index": pattern_index, "category": category,
                                          "classification": classification})


@mcp.tool()
def distribute_internal_lines(pattern_index: int, line_indices: list[int], number: int, straight: bool = True,
                              perpendicular: bool = False, graduate: bool = False) -> dict:
    """Distribute internal lines between segments, for pleats, tucks or slash lines.

    Args:
        pattern_index: Zero-based pattern index.
        line_indices: Segments between which the lines are spread.
        number: Number of lines.
        straight: Straight lines instead of curves.
        perpendicular: Perpendicular to the segments.
        graduate: Graduate along the segment lengths.
    """
    return _send("distribute_internal_lines", {"pattern_index": pattern_index, "line_indices": line_indices,
                                               "number": number, "straight": straight,
                                               "perpendicular": perpendicular, "graduate": graduate})


@mcp.tool()
def add_annotation(pattern_index: int, x: float, y: float, text: str) -> dict:
    """Write a text annotation on a pattern piece at (x, y) mm."""
    return _send("add_annotation", {"pattern_index": pattern_index, "x": x, "y": y, "text": text})


@mcp.tool()
def edit_annotation(pattern_index: int, annotation_index: int, x: float, y: float, text: str) -> dict:
    """Change the text or position of an annotation of a pattern piece."""
    return _send("edit_annotation", {"pattern_index": pattern_index, "annotation_index": annotation_index,
                                     "x": x, "y": y, "text": text})


@mcp.tool()
def get_annotations(pattern_index: int) -> dict:
    """List the annotations of a pattern piece with their positions."""
    return _send("get_annotations", {"pattern_index": pattern_index})


@mcp.tool()
def instance_pattern(pattern_index: int | None = None, name: str | None = None) -> dict:
    """Create a linked copy (instance) of a pattern piece: edits on one apply to the other."""
    return _send("instance_pattern", {"pattern_index": pattern_index, "name": name})


@mcp.tool()
def symmetry_pattern(pattern_index: int | None = None, name: str | None = None, flag: bool = True) -> dict:
    """Create the symmetric piece of a pattern piece, for example the left sleeve from the right one.

    Args:
        pattern_index: Zero-based pattern index, or give name.
        name: Pattern name.
        flag: Option of CLO's symmetry command, probably keeping the two pieces linked.
    """
    return _send("symmetry_pattern", {"pattern_index": pattern_index, "name": name, "flag": flag})


@mcp.tool()
def unfold_pattern_by_name(name: str, line_index: int, half_symmetry: bool = False) -> dict:
    """Unfold a half pattern piece, found by its name, along one of its lines."""
    return _send("unfold_pattern_by_name", {"name": name, "line_index": line_index, "half_symmetry": half_symmetry})


@mcp.tool()
def get_selected_patterns() -> dict:
    """Get the pattern pieces the user has selected in CLO3D, by name and index."""
    return _send("get_selected_patterns")


@mcp.tool()
def select_pattern(pattern_index: int | None = None, name: str | None = None, selected: bool = True) -> dict:
    """Select or deselect a pattern piece in CLO3D, by index or by name."""
    return _send("select_pattern", {"pattern_index": pattern_index, "name": name, "selected": selected})


@mcp.tool()
def get_scene_summary() -> dict:
    """Read the whole scene in one call, before acting on it: every pattern piece with its index, name, 2D
    position, bounding box, fabric index, layer, mesh size and arrangement point; the fabrics with their names;
    the avatars; and the arrangement points of the avatar with their indices and names."""
    return _send("get_scene_summary")
