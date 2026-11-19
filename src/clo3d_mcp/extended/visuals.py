"""Camera, images, stress and strain maps, technical drawing and colorways."""

from clo3d_mcp.server import _send, mcp


@mcp.tool()
def set_camera_view(view_index: int, zoom_to_garment: bool = False) -> dict:
    """Move the 3D camera.

    Args:
        view_index: 0 bottom, 1 three-quarter right, 2 front, 3 three-quarter left, 4 right,
                    5 top, 6 left, 7 focus zoom, 8 back, 9 zoom extents.
        zoom_to_garment: Also zoom on the garment.
    """
    return _send("set_camera_view", {"view_index": view_index, "zoom_to_garment": zoom_to_garment})


@mcp.tool()
def export_view_image(view_index: int, output_path: str, colorway_index: int = 0,
                      width: int = 1024, height: int = 1024) -> dict:
    """Save an image of the garment from a standard view, to look at the fit.

    Args:
        view_index: Standard view index.
        output_path: Absolute image path (.png). A folder gets a file named view_<index>.png.
        colorway_index: Colorway to render.
        width: Image width in pixels.
        height: Image height in pixels.
    """
    return _send("export_view_image", {"view_index": view_index, "output_path": output_path,
                                       "colorway_index": colorway_index, "width": width, "height": height})


@mcp.tool()
def export_custom_view_snapshot(output_folder: str, width: int = 1024, height: int = 1024,
                                prefix: str = "") -> dict:
    """Save snapshots of the custom views of the 3D window."""
    return _send("export_custom_view_snapshot", {"output_folder": output_folder, "width": width,
                                                 "height": height, "prefix": prefix})


@mcp.tool()
def export_rendering_image(file_path: str) -> dict:
    """Render the garment with the rendering settings of the project and save the images."""
    return _send("export_rendering_image", {"file_path": file_path})


@mcp.tool()
def set_fit_maps(stress: bool | None = None, strain: bool | None = None) -> dict:
    """Show or hide the stress map and the strain map, to see where the garment is too tight.

    Args:
        stress: True to show the stress map, False to hide it, None to leave it.
        strain: True to show the strain map, False to hide it, None to leave it.
    """
    return _send("set_fit_maps", {"stress": stress, "strain": strain})


@mcp.tool()
def set_schematic(render: bool | None = None, seam_lines: bool | None = None, internal_lines: bool | None = None,
                  topstitch_lines: bool | None = None, silhouette: bool | None = None,
                  seam_line_width: int | None = None, internal_line_width: int | None = None,
                  silhouette_width: int | None = None, topstitch_scale_percent: int | None = None) -> dict:
    """Show the garment as a flat technical drawing and choose which lines appear and how thick."""
    return _send("set_schematic", {"render": render, "seam_lines": seam_lines, "internal_lines": internal_lines,
                                   "topstitch_lines": topstitch_lines, "silhouette": silhouette,
                                   "seam_line_width": seam_line_width, "internal_line_width": internal_line_width,
                                   "silhouette_width": silhouette_width,
                                   "topstitch_scale_percent": topstitch_scale_percent})


@mcp.tool()
def export_view_images_all_colorways(view_index: int, output_path: str, width: int = 1024,
                                     height: int = 1024) -> dict:
    """Save an image of a standard view for every colorway (absolute image path, or a folder)."""
    return _send("export_view_images_all_colorways", {"view_index": view_index, "output_path": output_path,
                                                      "width": width, "height": height})


@mcp.tool()
def get_colorway_colors(colorway_index: int) -> dict:
    """List the color items of a colorway with their names and RGB values."""
    return _send("get_colorway_colors", {"colorway_index": colorway_index})


@mcp.tool()
def update_colorways(update_snapshot: bool = True) -> dict:
    """Refresh the colorways, and their snapshots if asked."""
    return _send("update_colorways", {"update_snapshot": update_snapshot})


@mcp.tool()
def export_snapshot_2d(output_path: str, option: int = 0) -> dict:
    """Save an image of the 2D pattern window, to look at the pieces (absolute .png path or folder).

    Args:
        output_path: Image path, or a folder that gets patterns_2d.png.
        option: Integer option of CLO's 2D snapshot, undocumented.
    """
    return _send("export_snapshot_2d", {"output_path": output_path, "option": option})
