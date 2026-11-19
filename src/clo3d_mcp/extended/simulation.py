"""Arrangement on the avatar, pattern states and simulation settings."""

from clo3d_mcp.server import _send, mcp


@mcp.tool()
def set_particle_distance(pattern_index: int, distance: float) -> dict:
    """Set the simulation mesh size of a pattern piece in mm. Smaller is finer and slower (default 20)."""
    return _send("set_particle_distance", {"pattern_index": pattern_index, "distance": distance})


@mcp.tool()
def get_pattern_arrangements() -> dict:
    """Get the arrangement point currently used by each pattern piece."""
    return _send("get_pattern_arrangements")


@mcp.tool()
def arrange_pattern(pattern_index: int, arrangement_index: int | None = None, x: int | None = None,
                    y: int | None = None, offset: int = 0, orientation: int | None = None,
                    shape_style: str | None = None) -> dict:
    """Place a pattern piece around the avatar before simulating.

    Args:
        pattern_index: Zero-based pattern index.
        arrangement_index: Arrangement point, from get_arrangement_list.
        x: Optional position on the arrangement, with y and offset.
        y: Optional position on the arrangement.
        offset: Distance from the avatar, with x and y.
        orientation: Optional orientation code.
        shape_style: Optional arrangement shape style.
    """
    return _send("arrange_pattern", {"pattern_index": pattern_index, "arrangement_index": arrangement_index,
                                     "x": x, "y": y, "offset": offset, "orientation": orientation,
                                     "shape_style": shape_style})


@mcp.tool()
def reset_cloth_arrangement() -> dict:
    """Restore the cloth shape to its state when it was loaded."""
    return _send("reset_cloth_arrangement")


@mcp.tool()
def set_pattern_state(pattern_index: int, layer: int | None = None, freeze: bool | None = None,
                      lock: bool | None = None, hide_3d: bool | None = None, solidify: bool | None = None,
                      solidify_strength: float | None = None, strengthen: bool | None = None,
                      grain_link_all_colorways: bool | None = None) -> dict:
    """Set the simulation state of a pattern piece. Only the given settings change.

    Args:
        pattern_index: Zero-based pattern index.
        layer: Layer number, higher layers go over lower ones (a jacket over a shirt).
        freeze: Freeze the piece in place.
        lock: Lock the piece against edits.
        hide_3d: Hide the piece in the 3D window.
        solidify: Make the piece rigid, as for a collar or a cuff.
        solidify_strength: Rigidity strength.
        strengthen: Reinforce the piece.
        grain_link_all_colorways: Share the grain direction across colorways.
    """
    return _send("set_pattern_state", {"pattern_index": pattern_index, "layer": layer, "freeze": freeze,
                                       "lock": lock, "hide_3d": hide_3d, "solidify": solidify,
                                       "solidify_strength": solidify_strength, "strengthen": strengthen,
                                       "grain_link_all_colorways": grain_link_all_colorways})


@mcp.tool()
def set_particle_distance_all(distance: float) -> dict:
    """Set the simulation mesh size of every pattern piece, in mm (default 20)."""
    return _send("set_particle_distance_all", {"distance": distance})


@mcp.tool()
def set_shrinkage(pattern_index: int, width_percent: float | None = None, height_percent: float | None = None) -> dict:
    """Set the fabric shrinkage of a pattern piece in percent, across and along the grain."""
    return _send("set_shrinkage", {"pattern_index": pattern_index, "width_percent": width_percent,
                                   "height_percent": height_percent})


@mcp.tool()
def get_simulation_quality() -> dict:
    """Get the simulation preset and mode."""
    return _send("get_simulation_quality")


@mcp.tool()
def set_simulation_settings(time_step: float | None = None, substeps: int | None = None,
                            cg_iterations: int | None = None, cg_residual: float | None = None,
                            cg_finish_condition: int | None = None, self_collision_iterations: int | None = None,
                            self_collision_stiffness: float | None = None, layer_based_collision: bool | None = None,
                            nonlinear: bool | None = None, air_damping: float | None = None,
                            gravity: float | None = None, ground_collision: bool | None = None,
                            ground_height: float | None = None, cpu_count: int | None = None) -> dict:
    """Advanced simulation settings. Only the given settings change.

    Args:
        time_step: Seconds per step (default 0.0333, min 0.001).
        substeps: Substeps per step (1 to 50).
        cg_iterations: Solver iterations per step (1 to 1000, default 50).
        cg_residual: Solver residual to stop (0.0001 normal, 0.00001 animation and fitting).
        cg_finish_condition: 0 by iterations, 1 by residual.
        self_collision_iterations: Self-collision iterations (default 1).
        self_collision_stiffness: Self-collision avoidance stiffness (default 0.001).
        layer_based_collision: Use pattern layers for collisions.
        nonlinear: Nonlinear simulation.
        air_damping: Air damping (default 1.0).
        gravity: Gravity on y in mm per second squared (default -9800).
        ground_collision: Collide with the ground.
        ground_height: Ground height on y.
        cpu_count: Number of CPU threads.
    """
    return _send("set_simulation_settings", {
        "time_step": time_step, "substeps": substeps, "cg_iterations": cg_iterations, "cg_residual": cg_residual,
        "cg_finish_condition": cg_finish_condition, "self_collision_iterations": self_collision_iterations,
        "self_collision_stiffness": self_collision_stiffness, "layer_based_collision": layer_based_collision,
        "nonlinear": nonlinear, "air_damping": air_damping, "gravity": gravity,
        "ground_collision": ground_collision, "ground_height": ground_height, "cpu_count": cpu_count})


@mcp.tool()
def set_mesh_type(pattern_index: int, mesh_type: str) -> dict:
    """Set the mesh type of a pattern piece and return its face and vertex counts."""
    return _send("set_mesh_type", {"pattern_index": pattern_index, "mesh_type": mesh_type})


@mcp.tool()
def get_mesh_counts(pattern_index: int) -> dict:
    """Get the face and vertex counts of a pattern piece by mesh type."""
    return _send("get_mesh_counts", {"pattern_index": pattern_index})


@mcp.tool()
def get_pin_count() -> dict:
    """Get the number of pins in the scene."""
    return _send("get_pin_count")


@mcp.tool()
def remove_pins(all: bool = True) -> dict:
    """Remove all pins, or only the selected pin when all is False."""
    return _send("remove_pins", {"all": all})


@mcp.tool()
def set_collision_thickness(pattern_index: int, thickness: float) -> dict:
    """Set the additional collision thickness of a pattern piece in mm, for thick fabrics or layers."""
    return _send("set_collision_thickness", {"pattern_index": pattern_index, "thickness": thickness})
