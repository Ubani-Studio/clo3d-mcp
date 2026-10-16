"""
CLO3D MCP Server: FastMCP server exposing CLO3D tools to LLMs.

Bridges Claude/Cursor and CLO3D via the Model Context Protocol.
Communicates with the CLO3D plugin through a shared file directory.
"""

from mcp.server.fastmcp import FastMCP
from clo3d_mcp.connection import get_connection, CLO3DConnectionError

mcp = FastMCP(
    "clo3d",
    instructions="Control CLO3D — the industry-standard 3D garment design software. "
    "Create patterns, manage fabrics, run simulations, export 3D models, and more.",
)


def _send(command: str, params: dict | None = None) -> dict:
    """Send a command to CLO3D and return the result."""
    conn = get_connection()
    return conn.send_command(command, params)


# ─── Scene Tools ───────────────────────────────────────────────────────────


@mcp.tool()
def get_project_info() -> dict:
    """Get information about the current CLO3D project including name, path, version, and counts for patterns, fabrics, and colorways."""
    return _send("get_project_info")


@mcp.tool()
def new_project() -> dict:
    """Create a new empty CLO3D project, clearing the current scene."""
    return _send("new_project")


@mcp.tool()
def open_file(file_path: str) -> dict:
    """Open a file in CLO3D. Supports .zprj, .zpac, .avt, .obj, .fbx formats.

    Args:
        file_path: Absolute path to the file to open.
    """
    return _send("open_file", {"file_path": file_path})


@mcp.tool()
def save_project(file_path: str) -> dict:
    """Save the current CLO3D project as a .zprj file.

    Args:
        file_path: Absolute path for the saved .zprj file.
    """
    return _send("save_file", {"file_path": file_path})


@mcp.tool()
def get_garment_info() -> dict:
    """Export and retrieve garment metadata as JSON, including pattern details, fabric assignments, and measurements."""
    return _send("get_garment_info")


# ─── Pattern Tools ─────────────────────────────────────────────────────────


@mcp.tool()
def get_pattern_count() -> dict:
    """Get the total number of pattern pieces in the current project."""
    return _send("get_pattern_count")


@mcp.tool()
def get_pattern_list() -> dict:
    """Get a list of all pattern pieces with their indices and names."""
    return _send("get_pattern_list")


@mcp.tool()
def get_pattern_info(pattern_index: int) -> dict:
    """Get detailed information about a specific pattern piece.

    Args:
        pattern_index: Zero-based index of the pattern piece.
    """
    return _send("get_pattern_info", {"pattern_index": pattern_index})


@mcp.tool()
def get_pattern_bounding_box(pattern_index: int) -> dict:
    """Get the bounding box (width/height) of a pattern piece.

    Args:
        pattern_index: Zero-based index of the pattern piece.
    """
    return _send("get_bounding_box", {"pattern_index": pattern_index})


@mcp.tool()
def set_pattern_name(pattern_index: int, name: str) -> dict:
    """Rename a pattern piece.

    Args:
        pattern_index: Zero-based index of the pattern piece.
        name: New name for the pattern piece.
    """
    return _send("set_pattern_name", {"pattern_index": pattern_index, "name": name})


@mcp.tool()
def copy_pattern(pattern_index: int, x: float = 0, y: float = 0) -> dict:
    """Duplicate a pattern piece at a given position.

    Args:
        pattern_index: Zero-based index of the pattern piece to copy.
        x: X position offset for the copy (mm).
        y: Y position offset for the copy (mm).
    """
    return _send("copy_pattern", {"pattern_index": pattern_index, "x": x, "y": y})


@mcp.tool()
def delete_pattern(pattern_index: int) -> dict:
    """Delete a pattern piece from the project.

    Args:
        pattern_index: Zero-based index of the pattern piece to delete.
    """
    return _send("delete_pattern", {"pattern_index": pattern_index})


@mcp.tool()
def flip_pattern(
    pattern_index: int, horizontal: bool = True, each: bool = True
) -> dict:
    """Flip a pattern piece horizontally or vertically.

    Args:
        pattern_index: Zero-based index of the pattern piece to flip.
        horizontal: True for horizontal flip, False for vertical flip.
        each: True flips each piece about its own axis; False flips the
            selection as a group.
    """
    return _send("flip_pattern", {
        "pattern_index": pattern_index, "horizontal": horizontal, "each": each,
    })


@mcp.tool()
def create_pattern(points: list[list[float]]) -> dict:
    """Create a new pattern piece from vertex points.

    Args:
        points: List of [x, y] or [x, y, type] coordinates in mm.
                Type: 0=straight (default), 2=spline, 3=bezier.
                Example: [[0,0], [100,0], [100,200], [0,200]]
    """
    return _send("create_pattern", {"points": points})


@mcp.tool()
def list_library(kind: str, contains: str | None = None) -> dict:
    """List avatars or fabrics in CLO's asset library, with their file paths.

    Args:
        kind: "Avatar" or "Fabric".
        contains: Case-insensitive filter on file names, e.g. "Denim", "Male".
    """
    return _send("list_library", {"kind": kind, "contains": contains})


@mcp.tool()
def load_avatar(avatar: str) -> dict:
    """Load an avatar into the scene. Required before arrange_pattern.

    Args:
        avatar: Library name or part of it ("Luka", "Jinho", "Mia",
                "MV2.1_Mannequin"), or an absolute .avt path.
                See list_library(kind="Avatar").
    """
    return _send("load_avatar", {"avatar": avatar})


@mcp.tool()
def list_arrangement_points(contains: str | None = None) -> dict:
    """List the avatar's arrangement points (where pieces get placed on the body).

    Names follow Body_Front_Center_3, Body_Back_Center_3, Body_Side_L,
    Arm_Outside_1_R, Leg_Skirt_Front, Neck_Back, etc. Needs an avatar loaded.

    Args:
        contains: Case-insensitive filter, e.g. "Body_Front", "Skirt", "Arm".
    """
    return _send("list_arrangement_points", {"contains": contains})


@mcp.tool()
def arrange_pattern(pattern_index: int, point: str, orientation: int | None = None) -> dict:
    """Place a pattern piece on the avatar at an arrangement point, ready to simulate.

    Front pieces go on Body_Front_* points, backs on Body_Back_*, sleeves on
    Arm_*, skirt/trouser pieces on Leg_* points.

    Args:
        pattern_index: Index of the pattern to place.
        point: Arrangement point name (e.g. "Body_Front_Center_3") or index.
        orientation: Optional rotation in degrees around the point.
    """
    return _send("arrange_pattern", {"pattern_index": pattern_index, "point": point, "orientation": orientation})


@mcp.tool()
def reset_garment() -> dict:
    """Move all pieces back to their arranged positions, undoing simulation.

    Use before re-simulating after changing patterns or seams.
    """
    return _send("reset_garment")


@mcp.tool()
def get_pattern_lines(pattern_index: int) -> dict:
    """Get a pattern's outline lines: index, start/end points, length and location.

    Line i runs from outline point i to point i+1, in the order the points were
    given to create_pattern. Use these indices with sew_patterns. Re-read after
    unfold_pattern, which renumbers lines.

    Args:
        pattern_index: Zero-based index of the pattern piece.
    """
    return _send("get_pattern_lines", {"pattern_index": pattern_index})


@mcp.tool()
def sew_patterns(
    pattern_a: int,
    line_a: int,
    pattern_b: int,
    line_b: int,
    direction_a: bool = True,
    direction_b: bool = True,
    internal_shape_b: int | None = None,
) -> dict:
    """Sew an outline line of one pattern to an outline line of another,
    or to a line of an internal shape on pattern B (pass internal_shape_b).

    Directions follow each line's start->end (see get_pattern_lines): True sews
    start to end, False end to start. Choose them so the ends that should meet
    line up, e.g. two side seams drawn in opposite directions use True/False.

    Args:
        pattern_a: Index of the first pattern.
        line_a: Line index on the first pattern.
        pattern_b: Index of the second pattern.
        line_b: Line index on the second pattern.
        direction_a: Sewing direction along line_a.
        direction_b: Sewing direction along line_b.
        internal_shape_b: Internal shape index on pattern B, as returned by
                add_internal_line; line_b then indexes that shape's lines.
                Use for patch pockets: draw the pocket outline on the panel
                with add_internal_line, then sew the pocket edges to it.
    """
    return _send("sew_patterns", {
        "pattern_a": pattern_a, "line_a": line_a,
        "pattern_b": pattern_b, "line_b": line_b,
        "direction_a": direction_a, "direction_b": direction_b,
        "internal_shape_b": internal_shape_b,
    })


@mcp.tool()
def unfold_pattern(pattern_index: int, line_index: int, half_symmetry: bool = False) -> dict:
    """Mirror a half pattern across one of its lines to make the full piece.

    Renumbers the outline lines; the result includes the new numbering.
    Unfold before sewing, or re-read lines afterwards.

    Args:
        pattern_index: Index of the pattern to unfold.
        line_index: Line to mirror across (usually the centre front/back).
        half_symmetry: Keep the halves linked so edits mirror.
    """
    return _send("unfold_pattern", {
        "pattern_index": pattern_index, "line_index": line_index, "half_symmetry": half_symmetry,
    })


@mcp.tool()
def move_pattern(pattern_index: int, dx: float, dy: float) -> dict:
    """Move a pattern piece in the 2D window by an offset in mm. Returns its new position.

    Args:
        pattern_index: Index of the pattern to move.
        dx: Horizontal offset in mm.
        dy: Vertical offset in mm.
    """
    return _send("move_pattern", {"pattern_index": pattern_index, "dx": dx, "dy": dy})


@mcp.tool()
def set_grain_direction(pattern_index: int, degrees: float) -> dict:
    """Set a pattern's fabric grain direction in degrees (0 = default, 45 = bias).

    Args:
        pattern_index: Index of the pattern.
        degrees: Grain angle in degrees.
    """
    return _send("set_grain_direction", {"pattern_index": pattern_index, "degrees": degrees})


@mcp.tool()
def add_internal_line(pattern_index: int, points: list[list[float]], closed: bool = False) -> dict:
    """Draw an internal line or shape inside a pattern (darts, fold lines, placement marks).

    Args:
        pattern_index: Index of the pattern to draw in.
        points: [x, y] or [x, y, type] in the pattern's mm coordinates.
                Type: 0=straight (default), 2=spline, 3=bezier.
        closed: Connect the last point back to the first.

    Returns internal_shape_index, for sew_patterns(internal_shape_b=...).
    """
    return _send("add_internal_line", {"pattern_index": pattern_index, "points": points, "closed": closed})


@mcp.tool()
def get_arrangement_list() -> dict:
    """Get the list of arrangement points on the avatar."""
    return _send("get_arrangement_list")


# ─── Fabric Tools ──────────────────────────────────────────────────────────


@mcp.tool()
def get_fabric_list() -> dict:
    """Get a list of all fabrics in the current project with their indices."""
    return _send("get_fabric_list")


@mcp.tool()
def add_fabric(file_path: str) -> dict:
    """Add a new fabric to the project from a .zfab or .jfab file.

    Args:
        file_path: Absolute path to the fabric file (.zfab or .jfab).
    """
    return _send("add_fabric", {"file_path": file_path})


@mcp.tool()
def replace_fabric(fabric_index: int, file_path: str) -> dict:
    """Replace an existing fabric with a new one from file.

    Args:
        fabric_index: Zero-based index of the fabric to replace.
        file_path: Absolute path to the replacement fabric file (.zfab).
    """
    return _send("replace_fabric", {"fabric_index": fabric_index, "file_path": file_path})


@mcp.tool()
def assign_fabric_to_pattern(
    fabric_index: int, pattern_index: int, assign_option: int = 1
) -> dict:
    """Assign a fabric to a pattern piece.

    Args:
        fabric_index: Zero-based index of the fabric.
        pattern_index: Zero-based index of the pattern piece.
        assign_option: 1=current colorway only, 2=all colorways (unlinked), 3=all colorways (linked).
    """
    return _send(
        "assign_fabric",
        {
            "fabric_index": fabric_index,
            "pattern_index": pattern_index,
            "assign_option": assign_option,
        },
    )


@mcp.tool()
def set_fabric_color(
    fabric_index: int, r: int = 255, g: int = 255, b: int = 255,
    a: int = 255, material_face: int = 0,
) -> dict:
    """Set the PBR base color of a fabric.

    Args:
        fabric_index: Zero-based index of the fabric.
        r: Red channel (0-255).
        g: Green channel (0-255).
        b: Blue channel (0-255).
        a: Alpha channel (0-255).
        material_face: Which face to colour - 0 front, 1 back, 2 side.
    """
    return _send("set_fabric_color", {
        "fabric_index": fabric_index, "r": r, "g": g, "b": b,
        "a": a, "material_face": material_face,
    })


@mcp.tool()
def get_fabric_for_pattern(pattern_index: int) -> dict:
    """Get which fabric is assigned to a pattern piece.

    Args:
        pattern_index: Zero-based index of the pattern piece.
    """
    return _send("get_fabric_for_pattern", {"pattern_index": pattern_index})


# ─── Export Tools ──────────────────────────────────────────────────────────


@mcp.tool()
def export_obj(file_path: str, options: dict | None = None) -> dict:
    """Export the garment as an OBJ file.

    Args:
        file_path: Absolute path for the exported .obj file.
        options: Optional export options (bExportGarment, bExportAvatar, bThin, scale, etc.).
    """
    params = {"file_path": file_path}
    if options:
        params["options"] = options
    return _send("export_obj", params)


@mcp.tool()
def export_fbx(file_path: str) -> dict:
    """Export the garment as an FBX file.

    Args:
        file_path: Absolute path for the exported .fbx file.
    """
    return _send("export_fbx", {"file_path": file_path})


@mcp.tool()
def export_glb(file_path: str, options: dict | None = None) -> dict:
    """Export the garment as a GLB (binary glTF) file.

    Args:
        file_path: Absolute path for the exported .glb file.
        options: Optional export options.
    """
    params = {"file_path": file_path}
    if options:
        params["options"] = options
    return _send("export_glb", params)


@mcp.tool()
def export_gltf(file_path: str, options: dict | None = None) -> dict:
    """Export the garment as a glTF file.

    Args:
        file_path: Absolute path for the exported .gltf file.
        options: Optional export options.
    """
    params = {"file_path": file_path}
    if options:
        params["options"] = options
    return _send("export_gltf", params)


@mcp.tool()
def export_thumbnail(file_path: str) -> dict:
    """Export a 3D viewport thumbnail.

    Args:
        file_path: Absolute path for the exported image file.

    CLO's ExportThumbnail3D takes no size arguments, so the previous
    width/height parameters could never have any effect.
    """
    return _send("export_thumbnail", {"file_path": file_path})


@mcp.tool()
def export_snapshot(file_path: str | None = None, views: list[str] | None = None) -> dict:
    """Export snapshot images of the 3D view. Returns the image file paths.

    Use after simulate to check the drape; the images can be viewed directly.
    Pass views to rotate the camera and capture several angles in one call;
    the camera returns to the front afterwards.

    Args:
        file_path: Absolute .png path (with views, used as the name prefix).
                   Default: timestamped files in ~/clo3d-mcp/.snapshots/.
        views: Camera angles: "front", "back", "left", "right",
               "three_quarter_left", "three_quarter_right", "top", "bottom".
               Omit to capture the current camera.
    """
    params = {}
    if file_path:
        params["file_path"] = file_path
    if views:
        params["views"] = views
    return _send("export_snapshot", params)


@mcp.tool()
def export_turntable(
    file_path: str,
    number_of_images: int = 36,
    width: int = 2500,
    height: int = 2500,
) -> dict:
    """Export a 360-degree turntable image sequence.

    Args:
        file_path: Absolute path (directory or base name) for turntable images.
        number_of_images: How many frames to render around the turn.
        width: Frame width in pixels.
        height: Frame height in pixels.
    """
    return _send("export_turntable", {
        "file_path": file_path, "number_of_images": number_of_images,
        "width": width, "height": height,
    })


@mcp.tool()
def export_tech_pack(file_path: str) -> dict:
    """Export a tech pack with JSON metadata and images.

    Args:
        file_path: Absolute path for the tech pack output.
    """
    return _send("export_tech_pack", {"file_path": file_path})


# ─── Import Tools ──────────────────────────────────────────────────────────


@mcp.tool()
def import_file(file_path: str) -> dict:
    """Import a file into CLO3D. Auto-detects type from extension (.zprj, .zpac, .obj, .fbx, .avt, etc.).

    Args:
        file_path: Absolute path to the file to import.
    """
    return _send("import_file", {"file_path": file_path})


@mcp.tool()
def import_dxf(file_path: str, options: dict | None = None) -> dict:
    """Import a DXF or DXF-AAMA pattern through CLO3D's DXF importer. Appends to the open project and keeps notches
    by default; options takes ImportDxfOption fields by name (m_bAppend, m_Scale, m_bConvertToSeamAllowance, ...).

    Args:
        file_path: Absolute path to the .dxf file.
        options: Optional ImportDxfOption overrides.
    """
    return _send("import_dxf", {"file_path": file_path, "options": options or {}})


# ─── Simulation Tools ─────────────────────────────────────────────────────


@mcp.tool()
def simulate(steps: int = 100) -> dict:
    """Run cloth simulation for a number of steps (about 2 s per 50 steps).

    Pieces must be sewn and arranged on an avatar first. Check the result
    with export_snapshot.

    Args:
        steps: Number of simulation steps to run (default 100).
    """
    return _send("simulate", {"steps": steps})


# ─── Colorway Tools ───────────────────────────────────────────────────────


@mcp.tool()
def get_colorways() -> dict:
    """Get a list of all colorways in the current project with names and which is active."""
    return _send("get_colorways")


@mcp.tool()
def set_current_colorway(colorway_index: int) -> dict:
    """Switch to a different colorway.

    Args:
        colorway_index: Zero-based index of the colorway to activate.
    """
    return _send("set_current_colorway", {"colorway_index": colorway_index})


@mcp.tool()
def set_colorway_name(colorway_index: int, name: str) -> dict:
    """Rename a colorway.

    Args:
        colorway_index: Zero-based index of the colorway to rename.
        name: New name for the colorway.
    """
    return _send("set_colorway_name", {"colorway_index": colorway_index, "name": name})


@mcp.tool()
def copy_colorway(colorway_index: int, copy_option: int = 0) -> dict:
    """Duplicate a colorway, returning the new colorway's index.

    Args:
        colorway_index: Zero-based index of the colorway to copy.
        copy_option: 0 = unlink all properties, 1 = unlink material properties
            only, 2 = link all properties to the source colorway.
    """
    return _send("copy_colorway",
                 {"colorway_index": colorway_index, "copy_option": copy_option})


@mcp.tool()
def delete_colorway(colorway_index: int) -> dict:
    """Delete a colorway from the project.

    Args:
        colorway_index: Zero-based index of the colorway to delete.
    """
    return _send("delete_colorway", {"colorway_index": colorway_index})


@mcp.tool()
def get_avatars() -> dict:
    """List the avatars in the current project, with names and genders."""
    return _send("get_avatars")


@mcp.tool()
def get_avatar_genders() -> dict:
    """Get the gender of each avatar in the current project."""
    return _send("get_avatar_genders")


@mcp.tool()
def show_hide_avatar(show: bool = True) -> dict:
    """Show or hide the avatar in the 3D viewport.

    Args:
        show: True to show the avatar, False to hide it.
    """
    return _send("show_hide_avatar", {"show": show})


@mcp.tool()
def import_avatar(file_path: str, apf_path: str = "") -> dict:
    """Import an avatar into the current project.

    Args:
        file_path: Absolute path to the avatar file (.avt).
        apf_path: Optional absolute path to an avatar pose file (.apf).
    """
    return _send("import_avatar", {"file_path": file_path, "apf_path": apf_path})


@mcp.tool()
def get_fabric_count() -> dict:
    """Get the number of fabrics in the current project."""
    return _send("get_fabric_count")


@mcp.tool()
def import_fabric(file_path: str) -> dict:
    """Import a fabric from file, returning its new fabric index.

    Args:
        file_path: Absolute path to the fabric file (.zfab).
    """
    return _send("import_fabric", {"file_path": file_path})


@mcp.tool()
def delete_fabric(fabric_index: int) -> dict:
    """Delete a fabric from the project.

    Args:
        fabric_index: Zero-based index of the fabric to delete.
    """
    return _send("delete_fabric", {"fabric_index": fabric_index})


@mcp.tool()
def set_simulation_quality(quality: int, simulation_mode: int = 0) -> dict:
    """Set the simulation quality preset.

    Args:
        quality: 0 = Normal (default), 1 = Animation (stable),
            2 = Fitting (accurate fabric), 3 = FAST (GPU).
        simulation_mode: 0 = CPU, 1 = FAST (GPU).
    """
    return _send("set_simulation_quality",
                 {"quality": quality, "simulation_mode": simulation_mode})


@mcp.tool()
def list_extensions() -> dict:
    """List the extension files loaded from CLO3D_MCP_EXTENSIONS, the tools each added, and any that
    failed to load with the reason. Empty when no extensions are configured."""
    return EXTENSIONS


@mcp.tool()
def ping() -> dict:
    """Check that the CLO3D bridge plug-in is running and reachable.

    Use this first when other tools time out - it distinguishes "the plug-in
    is not running" from "CLO is busy".
    """
    return _send("ping")


# ─── Generic API Tools ────────────────────────────────────────────────────


@mcp.tool()
def list_clo_api(module: str | None = None, contains: str | None = None, with_docs: bool = False) -> dict:
    """List the functions CLO3D's Python API exposes in this session.

    Use this to find a function before calling it with call_clo_api.

    Args:
        module: Limit to one module: export_api, fabric_api, import_api,
                pattern_api, utility_api or rest_api. Default: all available.
        contains: Case-insensitive substring filter on function names, e.g. "Seamline".
        with_docs: Include each function's docstring, which gives its exact
                Python signature. Use with a filter; the full API is large.
    """
    params = {"with_docs": with_docs}
    if module:
        params["module"] = module
    if contains:
        params["contains"] = contains
    return _send("list_api", params)


@mcp.tool()
def call_clo_api(
    module: str,
    function: str,
    args: list | None = None,
    inner_lists_as_tuples: bool = False,
) -> dict:
    """Call any function in CLO3D's Python API and return its result.

    Covers the whole API, not just the dedicated tools. Arguments are positional
    and must match CLO's signature (see list_clo_api and CLO's API reference).
    This can modify or overwrite files the same way the API can.

    Args:
        module: API module, e.g. "pattern_api".
        function: Function name, e.g. "AddSeamlinePairGroup".
        args: Positional arguments, e.g. [0, 1, 1, 3, True, False].
        inner_lists_as_tuples: Convert lists nested inside list arguments to
                tuples, for functions like CreatePatternWithPoints that take
                a list of (x, y, type) tuples.
    """
    return _send("call_api", {
        "module": module,
        "function": function,
        "args": args or [],
        "inner_lists_as_tuples": inner_lists_as_tuples,
    })


@mcp.tool()
def reload_plugin() -> dict:
    """Reload plugin/clo3d_mcp_plugin.py inside CLO3D to pick up edits without restarting CLO3D.

    Works when the plugin was loaded by the native pump (native/clo3d_mcp_pump.cpp).
    """
    return _send("reload_plugin")


# ─── Extensions ────────────────────────────────────────────────────────────
# Loaded last, so an extension sees every built-in tool name and cannot replace one.

from clo3d_mcp.extensions import load_extensions  # noqa: E402

EXTENSIONS = load_extensions(mcp, _send)
