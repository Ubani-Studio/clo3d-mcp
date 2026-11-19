"""File exchange and CLO's asset library."""

from clo3d_mcp.server import _send, mcp


@mcp.tool()
def export_pattern_json(file_path: str) -> dict:
    """Export the pattern pieces of the project to a JSON file (absolute path)."""
    return _send("export_pattern_json", {"file_path": file_path})


@mcp.tool()
def import_pattern_json(file_path: str) -> dict:
    """Load pattern pieces from a CLO3D pattern JSON file (absolute path).

    WARNING: this REPLACES the whole scene with the pieces of the file. Save the project first.
    Graphics (logos) are not stored in this JSON and are lost. Use it to build a new garment in one go:
    pieces, seams (SeamLinePairGroupList), notches (NotchList), buttons (ButtonHeadList) and buttonholes
    (ButtonHoleList), in the coordinates of the 2D window, as written by export_pattern_json.
    """
    return _send("import_pattern_json", {"file_path": file_path})


@mcp.tool()
def export_pom() -> dict:
    """Export the points of measure of the garment. Returns the file path in CLO's temporary folder."""
    return _send("export_pom")


@mcp.tool()
def export_garment_information(file_path: str | None = None) -> dict:
    """Save the garment information sheet to JSON. Without a path, it goes to CLO's temporary folder."""
    return _send("export_garment_information", {"file_path": file_path})


@mcp.tool()
def import_garment_information(file_path: str) -> dict:
    """Load a garment information sheet from JSON (absolute path)."""
    return _send("import_garment_information", {"file_path": file_path})


@mcp.tool()
def export_bom(file_path: str) -> dict:
    """Save the bill of materials: fabrics, trims and quantities (absolute path)."""
    return _send("export_bom", {"file_path": file_path})


@mcp.tool()
def export_zpac(file_path: str) -> dict:
    """Save the garment as a .zpac package (absolute path)."""
    return _send("export_zpac", {"file_path": file_path})


@mcp.tool()
def get_clo_folders() -> dict:
    """Get CLO's asset library, temporary and executable folders."""
    return _send("get_clo_folders")


@mcp.tool()
def find_assets(extensions: list[str], root: str | None = None, limit: int = 100) -> dict:
    """List files of CLO's asset library by extension, for example ["avt"] for avatars, ["zfab"] for fabrics."""
    return _send("find_assets", {"extensions": extensions, "root": root, "limit": limit})


@mcp.tool()
def build_garment(file_path: str | None = None, garment: dict | None = None, fabrics: dict | None = None,
                  arrange: bool = True, collision_thickness: float | None = 0, particle_distance: float | None = None,
                  allow_far: bool = False) -> dict:
    """Build a whole garment in one call from a garment JSON (pattern-making-skill's clo3d_<size>.json):
    pieces created and named, collision thickness, mesh, fabric by role, layer, arrangement point by name,
    then every seam and wrap hold. Returns the piece indices, the line-length deviation of each piece,
    the seams that failed, the pieces left unplaced and warnings.

    Garment JSON: {"pieces": [{"name", "points": [[x, y, type]...] in mm (y up, type 3 = Bezier control),
    "edges": [{"from": node, "length_mm"}...], "fabric": role, "particle_distance", "layer",
    "arrange": {"name": arrangement point name | "index", "orientation", "x", "y", "offset"}}],
    "seams": [[piece A, node a1, node a2, piece B, node b1, node b2, {"ease", "why"}]...],
    "holds": [{"top", "under", "edge": [a, b], "at": {a: [x, y], b: [x, y]}, "layer"}]}.

    Args:
        file_path: Absolute path of the garment JSON (CLO3D reads it), or pass `garment` inline.
        garment: The garment JSON as a dict, instead of file_path.
        fabrics: Fabric per role, e.g. {"shell": 2, "lining": "Cotton Twill", "*": 0}: index or name; "*" is
            the default for pieces whose role is not listed. Without it, pieces keep CLO3D's current fabric.
        arrange: Place pieces on their arrangement points (needs an avatar with arrangement points).
        collision_thickness: Added collision thickness for every piece, in mm; 0 by default because fabrics
            already carry their thickness. None leaves CLO3D's default.
        particle_distance: Mesh size in mm for pieces that do not set their own (None: CLO3D's default 20).
        allow_far: Accept points more than 2.5 m from the 2D origin (refused by default: pieces created that
            far fall for metres and have crashed CLO3D).
    """
    return _send("build_garment", {"file_path": file_path, "garment": garment, "fabrics": fabrics,
                                   "arrange": arrange, "collision_thickness": collision_thickness,
                                   "particle_distance": particle_distance, "allow_far": allow_far})
