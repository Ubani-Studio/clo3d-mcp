"""Avatars, measurements, poses and CLO's own grading sizes."""

from clo3d_mcp.server import _send, mcp


@mcp.tool()
def import_avatar_measurements(csv_path: str) -> dict:
    """Apply a measurement CSV file to the current avatar (absolute path)."""
    return _send("import_avatar_measurements", {"csv_path": csv_path})


@mcp.tool()
def get_grading_sizes() -> dict:
    """Get the sizes of CLO3D's own grading: groups, size names per group and the current size."""
    return _send("get_grading_sizes")


@mcp.tool()
def change_grading_size(group: int | str, size: int | str) -> dict:
    """Switch the garment to another size of CLO3D's grading. Give indices, or names, for both."""
    return _send("change_grading_size", {"group": group, "size": size})


@mcp.tool()
def get_grading_paired_avatar(grading_index: int) -> dict:
    """Get the name of the avatar paired with a grading size."""
    return _send("get_grading_paired_avatar", {"grading_index": grading_index})


@mcp.tool()
def import_pose(file_path: str) -> dict:
    """Load a body or hand pose file on the avatar (absolute path)."""
    return _send("import_pose", {"file_path": file_path})


@mcp.tool()
def export_pose(file_path: str | None = None) -> dict:
    """Save the avatar pose. Without a path, it goes to CLO's temporary folder."""
    return _send("export_pose", {"file_path": file_path})


@mcp.tool()
def align_avatar_and_garment() -> dict:
    """Move the avatars and the garment back to the center of the scene."""
    return _send("align_avatar_and_garment")


@mcp.tool()
def get_avatar_properties(avatar_index: int = 0) -> dict:
    """Get the properties of an avatar as key-value pairs."""
    return _send("get_avatar_properties", {"avatar_index": avatar_index})


@mcp.tool()
def set_avatar_properties(avatar_index: int, properties: dict[str, str]) -> dict:
    """Change properties of an avatar. Read get_avatar_properties first to know the keys."""
    return _send("set_avatar_properties", {"avatar_index": avatar_index, "properties": properties})


@mcp.tool()
def delete_avatars(avatar_indices: list[int]) -> dict:
    """Delete avatars from the scene."""
    return _send("delete_avatars", {"avatar_indices": avatar_indices})


@mcp.tool()
def set_avatar_activation(name: str, active: bool = True) -> dict:
    """Activate or deactivate an avatar by name."""
    return _send("set_avatar_activation", {"name": name, "active": active})


@mcp.tool()
def set_avatar_softness(avatar_index: int = 0, stiffness: float | None = None) -> dict:
    """Read, or set, the soft body stiffness of an avatar (how the body gives under tight garments)."""
    return _send("set_avatar_softness", {"avatar_index": avatar_index, "stiffness": stiffness})
