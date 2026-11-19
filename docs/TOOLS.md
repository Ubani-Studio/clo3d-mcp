# Tools

175 MCP tools, grouped by module of `src/clo3d_mcp`. Generated from the server code.

## Original tools (47)

Module: `server.py`

| Tool | Description |
|---|---|
| `add_fabric` | Add a new fabric to the project from a .zfab or .jfab file. |
| `assign_fabric_to_pattern` | Assign a fabric to a pattern piece. |
| `copy_colorway` | Duplicate a colorway, returning the new colorway's index. |
| `copy_pattern` | Duplicate a pattern piece at a given position. |
| `create_pattern` | Create a new pattern piece from vertex points. |
| `delete_colorway` | Delete a colorway from the project. |
| `delete_fabric` | Delete a fabric from the project. |
| `delete_pattern` | Delete a pattern piece from the project. |
| `export_fbx` | Export the garment as an FBX file. |
| `export_glb` | Export the garment as a GLB (binary glTF) file. |
| `export_gltf` | Export the garment as a glTF file. |
| `export_obj` | Export the garment as an OBJ file. |
| `export_snapshot` | Export multi-view snapshot images of the 3D garment. |
| `export_tech_pack` | Export a tech pack with JSON metadata and images. |
| `export_thumbnail` | Export a 3D viewport thumbnail. |
| `export_turntable` | Export a 360-degree turntable image sequence. |
| `flip_pattern` | Flip a pattern piece horizontally or vertically. |
| `get_arrangement_list` | Get the list of arrangement points on the avatar. |
| `get_avatar_genders` | Get the gender of each avatar in the current project. |
| `get_avatars` | List the avatars in the current project, with names and genders. |
| `get_colorways` | Get a list of all colorways in the current project with names and which is active. |
| `get_fabric_count` | Get the number of fabrics in the current project. |
| `get_fabric_for_pattern` | Get which fabric is assigned to a pattern piece. |
| `get_fabric_list` | Get a list of all fabrics in the current project with their indices. |
| `get_garment_info` | Export and retrieve garment metadata as JSON, including pattern details, fabric assignments, and measurements. |
| `get_pattern_bounding_box` | Get the bounding box (width/height) of a pattern piece. |
| `get_pattern_count` | Get the total number of pattern pieces in the current project. |
| `get_pattern_info` | Get detailed information about a specific pattern piece. |
| `get_pattern_list` | Get a list of all pattern pieces with their indices and names. |
| `get_project_info` | Get information about the current CLO3D project including name, path, version, and counts for patterns, fabrics, and colorways. |
| `import_avatar` | Import an avatar into the current project. |
| `import_dxf` | Import a DXF or DXF-AAMA pattern through CLO3D's DXF importer. Appends to the open project and keeps notches |
| `import_fabric` | Import a fabric from file, returning its new fabric index. |
| `import_file` | Import a file into CLO3D. Auto-detects type from extension (.zprj, .zpac, .obj, .fbx, .avt, etc.). |
| `list_extensions` | List the extension files loaded from CLO3D_MCP_EXTENSIONS, the tools each added, and any that |
| `new_project` | Create a new empty CLO3D project, clearing the current scene. |
| `open_file` | Open a file in CLO3D. Supports .zprj, .zpac, .avt, .obj, .fbx formats. |
| `ping` | Check that the CLO3D bridge plug-in is running and reachable. |
| `replace_fabric` | Replace an existing fabric with a new one from file. |
| `save_project` | Save the current CLO3D project as a .zprj file. |
| `set_colorway_name` | Rename a colorway. |
| `set_current_colorway` | Switch to a different colorway. |
| `set_fabric_color` | Set the PBR base color of a fabric. |
| `set_pattern_name` | Rename a pattern piece. |
| `set_simulation_quality` | Set the simulation quality preset. |
| `show_hide_avatar` | Show or hide the avatar in the 3D viewport. |
| `simulate` | Run cloth simulation for a number of steps. |

## Geometry: pieces, points, lines, annotations, scene summary (25)

Module: `extended/geometry.py`

| Tool | Description |
|---|---|
| `add_annotation` | Write a text annotation on a pattern piece at (x, y) mm. |
| `convert_shape` | Convert a shape of a pattern piece to an internal line or a base line. |
| `copy_pattern_with_offset` | Copy a pattern piece, shifted by (dx, dy) mm in the 2D window. Returns the new pattern index. |
| `create_internal_shape` | Draw a shape inside a pattern piece: dart lines, placement marks, topstitch guides. |
| `delete_line` | Delete a line of a pattern piece. |
| `delete_point` | Delete a point of a pattern piece. |
| `distribute_internal_lines` | Distribute internal lines between segments, for pleats, tucks or slash lines. |
| `edit_annotation` | Change the text or position of an annotation of a pattern piece. |
| `find_pattern` | Find a pattern index by its name, or by a scene position in the 2D or 3D window. |
| `get_annotations` | List the annotations of a pattern piece with their positions. |
| `get_line_lengths` | Get the length in mm of the first `count` outline lines of a pattern piece. |
| `get_pattern_details` | Get the 2D position, area and grain direction of a pattern piece. |
| `get_pattern_input_information` | Get the input data CLO3D keeps for a pattern piece, or for all pieces if no index is given. |
| `get_scene_summary` | Read the whole scene in one call, before acting on it: every pattern piece with its index, name, 2D |
| `get_selected_patterns` | Get the pattern pieces the user has selected in CLO3D, by name and index. |
| `instance_pattern` | Create a linked copy (instance) of a pattern piece: edits on one apply to the other. |
| `move_pattern_relative` | Move a pattern piece by (dx, dy) mm from its current position in the 2D window. |
| `offset_as_internal_line` | Create internal lines parallel to an outline line, for topstitching or facings. |
| `select_pattern` | Select or deselect a pattern piece in CLO3D, by index or by name. |
| `set_grain_direction` | Set the grain line direction of a pattern piece, in degrees (0 = vertical grain). |
| `set_pattern_category` | Set the category number or the classification text of a pattern piece (front, sleeve, collar...). |
| `set_pattern_position` | Move a pattern piece in the 2D window to (x, y), in mm. |
| `symmetry_pattern` | Create the symmetric piece of a pattern piece, for example the left sleeve from the right one. |
| `unfold_pattern` | Unfold a half pattern piece along one of its lines, as for a piece cut on the fold. |
| `unfold_pattern_by_name` | Unfold a half pattern piece, found by its name, along one of its lines. |

## Sewing: seams, topstitches, seam tape, shirring, elastic (14)

Module: `extended/sewing.py`

| Tool | Description |
|---|---|
| `add_seam_topstitch` | Add a topstitch along a seam, over all or part of its length. |
| `find_seam_group` | Find a seam group index by its name. |
| `get_all_stitches` | Get the properties of every stitch of the scene. |
| `get_seam_groups` | List the seam groups of the scene, and those attached to one pattern piece if given. |
| `get_topstitch_style_details` | List the topstitch styles with name, number of rows, distance, width and offset. |
| `get_topstitches` | List the topstitch styles, and the topstitches assigned to a pattern piece if an index is given. |
| `import_topstitch_style` | Import a topstitch style file (absolute path). |
| `set_elastic` | Put an elastic along a line of a pattern piece. |
| `set_seam_taping` | Put a seam tape along a line of a pattern piece (-1 for all lines), with an optional width in mm. |
| `set_shirring` | Turn gathers (shirring) on or off along a line of a pattern piece. |
| `set_topstitch` | Change a topstitch already placed on a pattern piece. Only the given settings change. |
| `set_topstitch_style` | Change a topstitch style: name, number of rows, distance between rows, thread width, offset. |
| `set_topstitch_style_color` | Set the thread color or opacity of a topstitch style. |
| `sew_lines` | Sew two lines together: outline to outline, outline to an internal shape, or internal to internal. |

## Simulation: arrangement, layers, stiffness, shrinkage, settings (14)

Module: `extended/simulation.py`

| Tool | Description |
|---|---|
| `arrange_pattern` | Place a pattern piece around the avatar before simulating. |
| `get_mesh_counts` | Get the face and vertex counts of a pattern piece by mesh type. |
| `get_pattern_arrangements` | Get the arrangement point currently used by each pattern piece. |
| `get_pin_count` | Get the number of pins in the scene. |
| `get_simulation_quality` | Get the simulation preset and mode. |
| `remove_pins` | Remove all pins, or only the selected pin when all is False. |
| `reset_cloth_arrangement` | Restore the cloth shape to its state when it was loaded. |
| `set_collision_thickness` | Set the additional collision thickness of a pattern piece in mm, for thick fabrics or layers. |
| `set_mesh_type` | Set the mesh type of a pattern piece and return its face and vertex counts. |
| `set_particle_distance` | Set the simulation mesh size of a pattern piece in mm. Smaller is finer and slower (default 20). |
| `set_particle_distance_all` | Set the simulation mesh size of every pattern piece, in mm (default 20). |
| `set_pattern_state` | Set the simulation state of a pattern piece. Only the given settings change. |
| `set_shrinkage` | Set the fabric shrinkage of a pattern piece in percent, across and along the grain. |
| `set_simulation_settings` | Advanced simulation settings. Only the given settings change. |

## Materials: fabrics, textures, graphics, trims, buttons, zippers (36)

Module: `extended/materials.py`

| Tool | Description |
|---|---|
| `add_graphic_to_pattern` | Place a graphic style (logo) on a pattern piece. Returns the positions of that logo on the piece. |
| `assign_graphic_style` | Give an existing graphic a different graphic style, for example another version of a logo. |
| `change_fabric_with_json` | Overwrite all the properties of a fabric, physics included, from a .jfab file (absolute path). |
| `duplicate_fabric` | Copy a fabric under a new name and return the new fabric index, for example to make a patch fabric. |
| `export_fabric` | Save a fabric to a .jfab or .zfab file. Without an index, the selected fabric is saved. |
| `export_trim` | Save a trim style to a file. |
| `find_fabric` | Find a fabric index by its name. |
| `find_trim_style` | Find a trim style index by its name. |
| `get_fabric_details` | Get the name, information, width and length in mm of a fabric. |
| `get_fabric_maps` | Get the color, normal and opacity image paths of a fabric. |
| `get_graphic_positions` | Get where a graphic style is placed: pattern index and point coordinates of each placement. |
| `get_graphic_style_maps` | Get the texture map paths of a graphic style. |
| `get_graphic_styles` | List the graphic styles of the current colorway with their names and dimensions, and the number of graphics. |
| `get_primary_fabric` | Get the fabric used by most pattern pieces. |
| `get_trim_material` | Get the material properties of a trim style mesh. |
| `get_trim_styles` | List the trim styles of the project. |
| `import_as_graphic` | Import an image as a graphic item in the scene (absolute path). |
| `import_graphic_style` | Import an image (logo, badge, print) as a graphic style of the project (absolute path, PNG or JPG). |
| `import_substance` | Load a Substance material (.sbsar, absolute path) onto a fabric, optionally on one face type. |
| `import_trim` | Import a trim: button, rivet, badge or other 3D accessory (absolute path). |
| `replace_graphic_style_image` | Replace the image of a graphic style, which updates every placement of that logo at once. |
| `set_button_colors` | Set the color of a button style and of a buttonhole style, four integers 0-255 each. |
| `set_current_fabric` | Select a fabric in the Object Browser. |
| `set_fabric` | Rename a fabric, set its width in mm, or its information fields (classification, content, supplier...). |
| `set_fabric_custom_image` | Set the custom thumbnail image of a fabric (.png or .jpg). |
| `set_fabric_displacement` | Set the displacement map of a fabric, and its amount in mm, shift, clipping and particle distance. |
| `set_fabric_maps` | Set texture images of a fabric (absolute paths). A logo as base_color makes a patch fabric. |
| `set_fabric_material` | Set the render material of a fabric face. Only the given settings change. |
| `set_graphic_style` | Rename, resize or recolor a graphic style. Every graphic using the style follows. |
| `set_graphic_style_maps` | Set texture maps of a graphic style, for example relief maps that make a logo look embroidered. |
| `set_pattern_fabric` | Use a fabric for a pattern piece. |
| `set_substance` | Choose the preset or the resolution of the Substance material of a fabric. |
| `set_texture_mapping` | Set how the fabric texture is mapped: repeat or unified (CLO type code). |
| `set_trim_material` | Change material properties of a trim style mesh. Read get_trim_material first to know the keys. |
| `set_trim_style` | Set the color (four integers 0-255, mesh -1 for all) or the weight of a trim style. |
| `set_zipper_style` | Change a zipper style: name, teeth, tape thickness, weight, slider, puller and stoppers. |

## Avatar, measurements, poses, grading (12)

Module: `extended/avatar.py`

| Tool | Description |
|---|---|
| `align_avatar_and_garment` | Move the avatars and the garment back to the center of the scene. |
| `change_grading_size` | Switch the garment to another size of CLO3D's grading. Give indices, or names, for both. |
| `delete_avatars` | Delete avatars from the scene. |
| `export_pose` | Save the avatar pose. Without a path, it goes to CLO's temporary folder. |
| `get_avatar_properties` | Get the properties of an avatar as key-value pairs. |
| `get_grading_paired_avatar` | Get the name of the avatar paired with a grading size. |
| `get_grading_sizes` | Get the sizes of CLO3D's own grading: groups, size names per group and the current size. |
| `import_avatar_measurements` | Apply a measurement CSV file to the current avatar (absolute path). |
| `import_pose` | Load a body or hand pose file on the avatar (absolute path). |
| `set_avatar_activation` | Activate or deactivate an avatar by name. |
| `set_avatar_properties` | Change properties of an avatar. Read get_avatar_properties first to know the keys. |
| `set_avatar_softness` | Read, or set, the soft body stiffness of an avatar (how the body gives under tight garments). |

## Files, whole garment, CLO library (10)

Module: `extended/files.py`

| Tool | Description |
|---|---|
| `build_garment` | Build a whole garment in one call from a garment JSON (pattern-making-skill's clo3d_<size>.json): |
| `export_bom` | Save the bill of materials: fabrics, trims and quantities (absolute path). |
| `export_garment_information` | Save the garment information sheet to JSON. Without a path, it goes to CLO's temporary folder. |
| `export_pattern_json` | Export the pattern pieces of the project to a JSON file (absolute path). |
| `export_pom` | Export the points of measure of the garment. Returns the file path in CLO's temporary folder. |
| `export_zpac` | Save the garment as a .zpac package (absolute path). |
| `find_assets` | List files of CLO's asset library by extension, for example ["avt"] for avatars, ["zfab"] for fabrics. |
| `get_clo_folders` | Get CLO's asset library, temporary and executable folders. |
| `import_garment_information` | Load a garment information sheet from JSON (absolute path). |
| `import_pattern_json` | Load pattern pieces from a CLO3D pattern JSON file (absolute path). |

## Views, images, maps, technical drawing, colorways (10)

Module: `extended/visuals.py`

| Tool | Description |
|---|---|
| `export_custom_view_snapshot` | Save snapshots of the custom views of the 3D window. |
| `export_rendering_image` | Render the garment with the rendering settings of the project and save the images. |
| `export_snapshot_2d` | Save an image of the 2D pattern window, to look at the pieces (absolute .png path or folder). |
| `export_view_image` | Save an image of the garment from a standard view, to look at the fit. |
| `export_view_images_all_colorways` | Save an image of a standard view for every colorway (absolute image path, or a folder). |
| `get_colorway_colors` | List the color items of a colorway with their names and RGB values. |
| `set_camera_view` | Move the 3D camera. |
| `set_fit_maps` | Show or hide the stress map and the strain map, to see where the garment is too tight. |
| `set_schematic` | Show the garment as a flat technical drawing and choose which lines appear and how thick. |
| `update_colorways` | Refresh the colorways, and their snapshots if asked. |

## Nesting (4)

Module: `extended/nesting.py`

| Tool | Description |
|---|---|
| `get_nesting_status` | Get the layout duration, the gap between pieces and the target colorway. |
| `set_nesting_pattern` | Fix a piece at (x, y) in the layout, or set how it may turn: 0 one way, 1 two ways, 2 four ways. |
| `start_nesting` | Start the automatic layout of the pieces on the fabric width, to measure fabric consumption. |
| `stop_nesting` | Stop the automatic layout. |

## API documentation (read-only) (3)

Module: `extended/docs.py`

| Tool | Description |
|---|---|
| `describe_clo_api` | Read the Python signatures of CLO API functions, without calling them. |
| `find_api_types` | Find names containing a text in every module loaded in CLO's Python, without calling them. |
| `list_clo_api` | List the public functions of every CLO Python API module available in this CLO3D build. |
