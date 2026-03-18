"""
CLO3D MCP Server: FastMCP server exposing CLO3D tools to LLMs.

Bridges Claude/Cursor and CLO3D via the Model Context Protocol.
Communicates with the CLO3D plugin through a shared file directory.
"""

from mcp.server.fastmcp import FastMCP
from clo3d_mcp.connection import get_connection, CLO3DConnectionError

mcp = FastMCP(
    "clo3d",
    description="Control CLO3D — the industry-standard 3D garment design software. "
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
def flip_pattern(pattern_index: int, horizontal: bool = True) -> dict:
    """Flip a pattern piece horizontally or vertically.

    Args:
        pattern_index: Zero-based index of the pattern piece to flip.
        horizontal: True for horizontal flip, False for vertical flip.
    """
    return _send("flip_pattern", {"pattern_index": pattern_index, "horizontal": horizontal})


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
def set_fabric_color(fabric_index: int, r: int = 255, g: int = 255, b: int = 255) -> dict:
    """Set the PBR base color of a fabric.

    Args:
        fabric_index: Zero-based index of the fabric.
        r: Red channel (0-255).
        g: Green channel (0-255).
        b: Blue channel (0-255).
    """
    return _send("set_fabric_color", {"fabric_index": fabric_index, "r": r, "g": g, "b": b})


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
def export_thumbnail(file_path: str, width: int = 512, height: int = 512) -> dict:
    """Export a 3D viewport screenshot/thumbnail.

    Args:
        file_path: Absolute path for the exported image file.
        width: Image width in pixels (default 512).
        height: Image height in pixels (default 512).
    """
    return _send("export_thumbnail", {"file_path": file_path, "width": width, "height": height})


@mcp.tool()
def export_snapshot(file_path: str) -> dict:
    """Export multi-view snapshot images of the 3D garment.

    Args:
        file_path: Absolute path (directory or base name) for snapshot images.
    """
    return _send("export_snapshot", {"file_path": file_path})


@mcp.tool()
def export_turntable(file_path: str) -> dict:
    """Export a 360-degree turntable image sequence.

    Args:
        file_path: Absolute path (directory or base name) for turntable images.
    """
    return _send("export_turntable", {"file_path": file_path})


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


# ─── Simulation Tools ─────────────────────────────────────────────────────


@mcp.tool()
def simulate(steps: int = 100) -> dict:
    """Run cloth simulation for a number of steps.

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
