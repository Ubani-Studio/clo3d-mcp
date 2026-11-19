"""Fabrics and their materials, graphics (logos), trims, buttons and zippers."""

from clo3d_mcp.server import _send, mcp


# ─── Graphics: logos, badges, prints ──────────────────────────────────────


@mcp.tool()
def import_graphic_style(image_path: str) -> dict:
    """Import an image (logo, badge, print) as a graphic style of the project (absolute path, PNG or JPG)."""
    return _send("import_graphic_style", {"image_path": image_path})


@mcp.tool()
def import_as_graphic(image_path: str) -> dict:
    """Import an image as a graphic item in the scene (absolute path)."""
    return _send("import_as_graphic", {"image_path": image_path})


@mcp.tool()
def get_graphic_styles() -> dict:
    """List the graphic styles of the current colorway with their names and dimensions, and the number of graphics."""
    return _send("get_graphic_styles")


@mcp.tool()
def set_graphic_style(style_index: int, name: str | None = None, width: float | None = None,
                      height: float | None = None, only_this_side: bool = False,
                      rgba: list[int] | None = None, colorway_index: int | None = None) -> dict:
    """Rename, resize or recolor a graphic style. Every graphic using the style follows.

    Args:
        style_index: Zero-based graphic style index.
        name: Optional new name.
        width: Optional width in mm. With height, sets both.
        height: Optional height in mm.
        only_this_side: When only width or height is given, change that side alone instead of keeping the ratio.
        rgba: Optional base color, four integers 0-255.
        colorway_index: Optional colorway for the color.
    """
    return _send("set_graphic_style", {"style_index": style_index, "name": name, "width": width,
                                       "height": height, "only_this_side": only_this_side,
                                       "rgba": rgba, "colorway_index": colorway_index})


@mcp.tool()
def replace_graphic_style_image(style_index: int, image_path: str, anchor: int = 0) -> dict:
    """Replace the image of a graphic style, which updates every placement of that logo at once.

    Args:
        style_index: Zero-based graphic style index.
        image_path: Absolute path of the new image.
        anchor: Point kept fixed when the size changes.
    """
    return _send("replace_graphic_style_image", {"style_index": style_index, "image_path": image_path,
                                                 "anchor": anchor})


@mcp.tool()
def set_graphic_style_maps(style_index: int, base_color: str | None = None, normal: str | None = None,
                           displacement: str | None = None, opacity: str | None = None,
                           roughness: str | None = None, metalness: str | None = None,
                           colorway_index: int | None = None) -> dict:
    """Set texture maps of a graphic style, for example relief maps that make a logo look embroidered.

    Args:
        style_index: Zero-based graphic style index.
        base_color: Optional absolute image path for the color map.
        normal: Optional normal map.
        displacement: Optional displacement map.
        opacity: Optional opacity map.
        roughness: Optional roughness map.
        metalness: Optional metalness map.
        colorway_index: Optional colorway for the color map.
    """
    return _send("set_graphic_style_maps", {"style_index": style_index, "base_color": base_color,
                                            "normal": normal, "displacement": displacement, "opacity": opacity,
                                            "roughness": roughness, "metalness": metalness,
                                            "colorway_index": colorway_index})


@mcp.tool()
def get_graphic_style_maps(style_index: int) -> dict:
    """Get the texture map paths of a graphic style."""
    return _send("get_graphic_style_maps", {"style_index": style_index})


@mcp.tool()
def assign_graphic_style(style_index: int, graphic_index: int, assign_option: int = 0,
                         colorway_index: int | None = None) -> dict:
    """Give an existing graphic a different graphic style, for example another version of a logo."""
    return _send("assign_graphic_style", {"style_index": style_index, "graphic_index": graphic_index,
                                          "assign_option": assign_option, "colorway_index": colorway_index})


# ─── Fabrics ──────────────────────────────────────────────────────────────


@mcp.tool()
def change_fabric_with_json(fabric_index: int, jfab_path: str, colorway_index: int | None = None) -> dict:
    """Overwrite all the properties of a fabric, physics included, from a .jfab file (absolute path)."""
    return _send("change_fabric_with_json", {"fabric_index": fabric_index, "jfab_path": jfab_path,
                                             "colorway_index": colorway_index})


@mcp.tool()
def export_fabric(file_path: str, fabric_index: int | None = None, colorway_index: int | None = None) -> dict:
    """Save a fabric to a .jfab or .zfab file. Without an index, the selected fabric is saved."""
    return _send("export_fabric", {"file_path": file_path, "fabric_index": fabric_index,
                                   "colorway_index": colorway_index})


@mcp.tool()
def get_fabric_details(fabric_index: int, colorway_index: int | None = None) -> dict:
    """Get the name, information, width and length in mm of a fabric."""
    return _send("get_fabric_details", {"fabric_index": fabric_index, "colorway_index": colorway_index})


@mcp.tool()
def set_fabric(fabric_index: int, name: str | None = None, width_mm: float | None = None,
               information: dict[str, str] | None = None, colorway_index: int | None = None) -> dict:
    """Rename a fabric, set its width in mm, or its information fields (classification, content, supplier...)."""
    return _send("set_fabric", {"fabric_index": fabric_index, "name": name, "width_mm": width_mm,
                                "information": information, "colorway_index": colorway_index})


@mcp.tool()
def find_fabric(name: str, colorway_index: int | None = None) -> dict:
    """Find a fabric index by its name."""
    return _send("find_fabric", {"name": name, "colorway_index": colorway_index})


@mcp.tool()
def set_pattern_fabric(pattern_index: int, fabric_index: int) -> dict:
    """Use a fabric for a pattern piece."""
    return _send("set_pattern_fabric", {"pattern_index": pattern_index, "fabric_index": fabric_index})


@mcp.tool()
def get_primary_fabric() -> dict:
    """Get the fabric used by most pattern pieces."""
    return _send("get_primary_fabric")


# ─── Trims and buttons ────────────────────────────────────────────────────


@mcp.tool()
def import_trim(file_path: str, translate_to_origin: bool = True) -> dict:
    """Import a trim: button, rivet, badge or other 3D accessory (absolute path)."""
    return _send("import_trim", {"file_path": file_path, "translate_to_origin": translate_to_origin})


@mcp.tool()
def export_trim(file_path: str, trim_style_index: int) -> dict:
    """Save a trim style to a file."""
    return _send("export_trim", {"file_path": file_path, "trim_style_index": trim_style_index})


@mcp.tool()
def set_trim_style(style_index: int, rgba: list[int] | None = None, mesh_index: int = -1,
                   weight: float | None = None, colorway_index: int | None = None) -> dict:
    """Set the color (four integers 0-255, mesh -1 for all) or the weight of a trim style."""
    return _send("set_trim_style", {"style_index": style_index, "rgba": rgba, "mesh_index": mesh_index,
                                    "weight": weight, "colorway_index": colorway_index})


@mcp.tool()
def set_button_colors(head_style_index: int | None = None, head_rgba: list[int] | None = None,
                      hole_style_index: int | None = None, hole_rgba: list[int] | None = None,
                      mesh_index: int = -1, colorway_index: int | None = None) -> dict:
    """Set the color of a button style and of a buttonhole style, four integers 0-255 each."""
    return _send("set_button_colors", {"head_style_index": head_style_index, "head_rgba": head_rgba,
                                       "hole_style_index": hole_style_index, "hole_rgba": hole_rgba,
                                       "mesh_index": mesh_index, "colorway_index": colorway_index})


@mcp.tool()
def get_trim_styles(flag: bool = True) -> dict:
    """List the trim styles of the project."""
    return _send("get_trim_styles", {"flag": flag})


@mcp.tool()
def find_trim_style(name: str) -> dict:
    """Find a trim style index by its name."""
    return _send("find_trim_style", {"name": name})


@mcp.tool()
def get_trim_material(style_index: int, mesh_index: int = 0) -> dict:
    """Get the material properties of a trim style mesh."""
    return _send("get_trim_material", {"style_index": style_index, "mesh_index": mesh_index})


@mcp.tool()
def set_trim_material(style_index: int, properties: dict[str, str], mesh_index: int = 0) -> dict:
    """Change material properties of a trim style mesh. Read get_trim_material first to know the keys."""
    return _send("set_trim_material", {"style_index": style_index, "mesh_index": mesh_index,
                                       "properties": properties})


@mcp.tool()
def set_zipper_style(style_index: int, name: str | None = None, teeth_type: int | None = None,
                     teeth_width: float | None = None, tape_thickness: float | None = None,
                     weight: float | None = None, slider: str | None = None, puller: str | None = None,
                     top_stopper: str | None = None, bottom_stopper: str | None = None) -> dict:
    """Change a zipper style: name, teeth, tape thickness, weight, slider, puller and stoppers."""
    return _send("set_zipper_style", {"style_index": style_index, "name": name, "teeth_type": teeth_type,
                                      "teeth_width": teeth_width, "tape_thickness": tape_thickness,
                                      "weight": weight, "slider": slider, "puller": puller,
                                      "top_stopper": top_stopper, "bottom_stopper": bottom_stopper})


@mcp.tool()
def add_graphic_to_pattern(pattern_index: int, style_index: int, x: float, y: float, width: float, height: float,
                           face: int = 0, z_offset: float = 0.0, angle: float = 0.0) -> dict:
    """Place a graphic style (logo) on a pattern piece. Returns the positions of that logo on the piece.

    Known limits with CLO 2025.2 (verified live): works on pieces drawn in CLO, was refused on pieces created
    by script; the position is approximate and placed logos cannot be moved or deleted through the API.
    Before placing, read the piece position and size (get_pattern_details, get_bounding_box) and capture the
    2D view (export_snapshot_2d); capture it again after placing to check where the logo landed.

    Args:
        pattern_index: Pattern piece receiving the logo.
        style_index: Graphic style index, from get_graphic_styles.
        x: Position of the logo center, from the left of the piece bounding box, in mm.
        y: Position of the logo center, from the top of the piece bounding box, downward, in mm.
        width: Logo width in mm.
        height: Logo height in mm.
        face: 0 front, 1 back, 2 both.
        z_offset: Depth offset.
        angle: Rotation in degrees.
    """
    return _send("add_graphic_to_pattern", {"pattern_index": pattern_index, "style_index": style_index,
                                            "x": x, "y": y, "width": width, "height": height,
                                            "face": face, "z_offset": z_offset, "angle": angle})


@mcp.tool()
def get_graphic_positions(style_index: int, flag: bool = True) -> dict:
    """Get where a graphic style is placed: pattern index and point coordinates of each placement."""
    return _send("get_graphic_positions", {"style_index": style_index, "flag": flag})


# ─── Fabric materials and sewn patches ────────────────────────────────────


@mcp.tool()
def set_current_fabric(fabric_index: int, colorway_index: int | None = None) -> dict:
    """Select a fabric in the Object Browser."""
    return _send("set_current_fabric", {"fabric_index": fabric_index, "colorway_index": colorway_index})


@mcp.tool()
def set_fabric_maps(fabric_index: int, base_color: str | None = None, normal: str | None = None,
                    displacement: str | None = None, opacity: str | None = None, roughness: str | None = None,
                    metalness: str | None = None, colorway_index: int | None = None) -> dict:
    """Set texture images of a fabric (absolute paths). A logo as base_color makes a patch fabric."""
    return _send("set_fabric_maps", {"fabric_index": fabric_index, "base_color": base_color, "normal": normal,
                                     "displacement": displacement, "opacity": opacity, "roughness": roughness,
                                     "metalness": metalness, "colorway_index": colorway_index})


@mcp.tool()
def get_fabric_maps(fabric_index: int, colorway_index: int | None = None) -> dict:
    """Get the color, normal and opacity image paths of a fabric."""
    return _send("get_fabric_maps", {"fabric_index": fabric_index, "colorway_index": colorway_index})


@mcp.tool()
def set_fabric_material(fabric_index: int, face: int = 0, material_type: int | None = None,
                        metalness: float | None = None, roughness_type: int | None = None,
                        roughness_intensity: int | None = None, roughness_map_intensity: int | None = None,
                        roughness_map_invert: bool | None = None, reflection_intensity: float | None = None,
                        reflection_roughness: float | None = None, normal_intensity: float | None = None,
                        opacity_intensity: float | None = None, same_color_as_front: bool | None = None,
                        same_material_as_front: bool | None = None, colorway_index: int | None = None) -> dict:
    """Set the render material of a fabric face. Only the given settings change.

    Args:
        fabric_index: Fabric index.
        face: 0 front, 1 back, 2 side.
        material_type: CLO material type code.
        metalness: Metalness value.
        roughness_type: CLO roughness type code.
        roughness_intensity: Roughness intensity.
        roughness_map_intensity: Roughness map intensity.
        roughness_map_invert: Invert the roughness map.
        reflection_intensity: Reflection intensity.
        reflection_roughness: Reflection roughness.
        normal_intensity: Normal map intensity in percent.
        opacity_intensity: Opacity intensity in percent.
        same_color_as_front: Back or side uses the front color.
        same_material_as_front: Back or side uses the front material.
        colorway_index: Optional colorway.
    """
    return _send("set_fabric_material", {
        "fabric_index": fabric_index, "face": face, "material_type": material_type, "metalness": metalness,
        "roughness_type": roughness_type, "roughness_intensity": roughness_intensity,
        "roughness_map_intensity": roughness_map_intensity, "roughness_map_invert": roughness_map_invert,
        "reflection_intensity": reflection_intensity, "reflection_roughness": reflection_roughness,
        "normal_intensity": normal_intensity, "opacity_intensity": opacity_intensity,
        "same_color_as_front": same_color_as_front, "same_material_as_front": same_material_as_front,
        "colorway_index": colorway_index})


@mcp.tool()
def set_fabric_displacement(fabric_index: int, image_path: str | None = None, amount: float | None = None,
                            shift: float = 0.0, clipping: float = 0.0, particle_distance: float = 5.0,
                            keep: bool = True, colorway_index: int | None = None) -> dict:
    """Set the displacement map of a fabric, and its amount in mm, shift, clipping and particle distance."""
    return _send("set_fabric_displacement", {"fabric_index": fabric_index, "image_path": image_path,
                                             "amount": amount, "shift": shift, "clipping": clipping,
                                             "particle_distance": particle_distance, "keep": keep,
                                             "colorway_index": colorway_index})


@mcp.tool()
def set_texture_mapping(fabric_index: int, mapping_type: int, colorway_index: int | None = None) -> dict:
    """Set how the fabric texture is mapped: repeat or unified (CLO type code)."""
    return _send("set_texture_mapping", {"fabric_index": fabric_index, "mapping_type": mapping_type,
                                         "colorway_index": colorway_index})


@mcp.tool()
def import_substance(fabric_index: int, file_path: str, face_type: str | None = None,
                     colorway_index: int | None = None) -> dict:
    """Load a Substance material (.sbsar, absolute path) onto a fabric, optionally on one face type."""
    return _send("import_substance", {"fabric_index": fabric_index, "file_path": file_path,
                                      "face_type": face_type, "colorway_index": colorway_index})


@mcp.tool()
def set_substance(fabric_index: int, face: int = 0, preset_index: int | None = None,
                  resolution_index: int | None = None, colorway_index: int | None = None) -> dict:
    """Choose the preset or the resolution of the Substance material of a fabric."""
    return _send("set_substance", {"fabric_index": fabric_index, "face": face, "preset_index": preset_index,
                                   "resolution_index": resolution_index, "colorway_index": colorway_index})


@mcp.tool()
def set_fabric_custom_image(fabric_index: int, image_path: str, colorway_index: int | None = None) -> dict:
    """Set the custom thumbnail image of a fabric (.png or .jpg)."""
    return _send("set_fabric_custom_image", {"fabric_index": fabric_index, "image_path": image_path,
                                             "colorway_index": colorway_index})


@mcp.tool()
def duplicate_fabric(fabric_index: int, name: str, folder: str | None = None) -> dict:
    """Copy a fabric under a new name and return the new fabric index, for example to make a patch fabric."""
    return _send("duplicate_fabric", {"fabric_index": fabric_index, "name": name, "folder": folder})
