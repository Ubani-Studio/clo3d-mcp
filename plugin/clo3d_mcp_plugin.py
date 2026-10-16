import builtins
import json
import os
import tempfile
import sys
import time

try:
    import export_api
    import fabric_api
    import import_api
    import pattern_api
    import utility_api
    IN_CLO3D = True
except ImportError:
    IN_CLO3D = False
    print("[CLO MCP] WARNING: Not running inside CLO3D. API calls will fail.")

# Communication directory
# Must match clo3d_mcp/connection.py. TEMP is a Windows variable: on macOS and
# Linux it is unset, and the two sides previously fell back to different
# directories (server /tmp, plug-in ~) so they never found each other.
# The native pump (native/clo3d_mcp_pump.cpp) runs inside CLO3D, which does not
# see the MCP server's environment, so it uses ~/clo3d_mcp: set CLO3D_MCP_DIR to
# that on the server side.
if os.environ.get("CLO3D_MCP_DIR"):
    COMM_DIR = os.environ["CLO3D_MCP_DIR"]
elif getattr(builtins, "_CLO3D_MCP_NATIVE_PUMP", False):
    COMM_DIR = os.path.join(os.path.expanduser("~"), "clo3d_mcp")
else:
    COMM_DIR = os.path.join(tempfile.gettempdir(), "clo3d_mcp")
ASSET_DIR = os.path.join(os.path.expanduser("~"), "Documents", "clo", "CLO Assets")
SNAPSHOT_DIR = os.path.join(os.path.expanduser("~"), "clo3d-mcp", ".snapshots")
POLL_INTERVAL = 0.1


# ---------------------------------------------------------------------------
# Command handlers
# ---------------------------------------------------------------------------

def handle_ping(params):
    reply = {"pong": True, "in_clo3d": IN_CLO3D}
    if globals().get("DEV"):
        reply["dev"] = True
        reply["plugin_mtime"] = os.path.getmtime(PLUGIN_PATH) if PLUGIN_PATH and os.path.isfile(PLUGIN_PATH) else None
    return reply


# -- Scene --

def handle_get_project_info(params):
    name = utility_api.GetProjectName()
    path = utility_api.GetProjectFilePath()
    major = utility_api.GetMajorVersion()
    minor = utility_api.GetMinorVersion()
    patch = utility_api.GetPatchVersion()
    pattern_count = pattern_api.GetPatternCount()
    fabric_count = fabric_api.GetFabricCount(True)
    colorway_count = utility_api.GetColorwayCount()
    return {
        "project_name": name,
        "project_path": path,
        "clo_version": str(major) + "." + str(minor) + "." + str(patch),
        "pattern_count": pattern_count,
        "fabric_count": fabric_count,
        "colorway_count": colorway_count,
    }


def handle_new_project(params):
    utility_api.NewProject()
    return {"created": True}


def handle_open_file(params):
    file_path = params["file_path"]
    result = import_api.ImportFile(file_path)
    return {"opened": result, "file_path": file_path}


def handle_save_file(params):
    file_path = params["file_path"]
    result = export_api.ExportZPrj(file_path)
    return {"saved": bool(result), "file_path": result or file_path}


def handle_get_garment_info(params):
    info_str = export_api.ExportGarmentInformationToStream()
    if info_str:
        try:
            data = json.loads(info_str)
        except (json.JSONDecodeError, TypeError):
            data = {"raw": str(info_str)}
        return {"garment_info": data}
    return {"garment_info": None}


# -- Pattern --

def handle_get_pattern_count(params):
    count = pattern_api.GetPatternCount()
    return {"count": count}


def handle_get_pattern_list(params):
    count = pattern_api.GetPatternCount()
    patterns = []
    for i in range(count):
        name = pattern_api.GetPatternPieceName(i)
        patterns.append({"index": i, "name": name})
    return {"patterns": patterns, "count": count}


def handle_get_pattern_info(params):
    index = params["pattern_index"]
    info_str = pattern_api.GetPatternInformation(index)
    try:
        info = json.loads(info_str) if info_str else {}
    except (json.JSONDecodeError, TypeError):
        info = {"raw": str(info_str)}
    name = pattern_api.GetPatternPieceName(index)
    return {"index": index, "name": name, "info": info}


def handle_get_bounding_box(params):
    index = params["pattern_index"]
    bb = pattern_api.GetBoundingBoxOfPattern(index)
    return {"index": index, "bounding_box": bb}


def handle_set_pattern_name(params):
    index = params["pattern_index"]
    name = params["name"]
    pattern_api.SetPatternPieceName(index, name)
    return {"index": index, "name": name}


def handle_copy_pattern(params):
    index = params["pattern_index"]
    x = params.get("x", 0)
    y = params.get("y", 0)
    pattern_api.CopyPatternPiecePos(index, x, y)
    return {"copied": True, "source_index": index, "position": [x, y]}


def handle_delete_pattern(params):
    index = params["pattern_index"]
    pattern_api.DeletePatternPiece(index)
    return {"deleted": True, "index": index}


def handle_flip_pattern(params):
    index = params["pattern_index"]
    horizontal = params.get("horizontal", True)
    each = params.get("each", True)
    pattern_api.FlipPatternPiece(index, horizontal, each)
    return {"flipped": True, "index": index, "horizontal": horizontal, "each": each}


def handle_create_pattern(params):
    points = params["points"]
    point_tuples = []
    for p in points:
        x, y = p[0], p[1]
        vtype = p[2] if len(p) > 2 else 0
        # CreatePatternWithPoints takes vector<tuple<float,float,int>>. pybind11
        # invokes NESTED casters with convert=False, so a Python int in the x/y
        # slots is rejected outright instead of promoted. Coerce explicitly.
        point_tuples.append((float(x), float(y), int(vtype)))
    result = pattern_api.CreatePatternWithPoints(point_tuples)
    return {"created": True, "point_count": len(point_tuples), "result": result}


def handle_get_arrangement_list(params):
    arr_list = pattern_api.GetArrangementList()
    return {"arrangements": arr_list}


# -- Fabric --

def handle_get_fabric_count(params):
    count = fabric_api.GetFabricCount(True)
    return {"count": count}


def handle_get_fabric_list(params):
    count = fabric_api.GetFabricCount(True)
    fabrics = []
    for i in range(count):
        fabrics.append({"index": i})
    return {"fabrics": fabrics, "count": count}


def handle_add_fabric(params):
    file_path = params["file_path"]
    index = fabric_api.AddFabric(file_path)
    return {"added": True, "fabric_index": index, "file_path": file_path}


def handle_assign_fabric(params):
    fabric_index = params["fabric_index"]
    pattern_index = params["pattern_index"]
    option = params.get("assign_option", 1)
    result = fabric_api.AssignFabricToPattern(fabric_index, pattern_index, option)
    return {"assigned": result, "fabric_index": fabric_index, "pattern_index": pattern_index}


def handle_set_fabric_color(params):
    fabric_index = params["fabric_index"]
    r = params.get("r", 255)
    g = params.get("g", 255)
    b = params.get("b", 255)
    a = params.get("a", 255)
    material_face = params.get("material_face", 0)
    # Convert 0-255 int range to 0.0-1.0 float range
    r_f = r / 255.0
    g_f = g / 255.0
    b_f = b / 255.0
    a_f = a / 255.0
    fabric_api.SetFabricPBRMaterialBaseColor(fabric_index, material_face, r_f, g_f, b_f, a_f)
    return {"set": True, "fabric_index": fabric_index, "color": [r, g, b, a]}


def handle_get_fabric_for_pattern(params):
    pattern_index = params["pattern_index"]
    fabric_index = fabric_api.GetFabricIndexForPattern(pattern_index)
    return {"pattern_index": pattern_index, "fabric_index": fabric_index}


def handle_replace_fabric(params):
    """fabric_api.ReplaceFabric(fabricIndex, inputFilePath) -> bool."""
    fabric_index = params["fabric_index"]
    file_path = params["file_path"]
    result = fabric_api.ReplaceFabric(fabric_index, file_path)
    return {"replaced": bool(result), "fabric_index": fabric_index,
            "file_path": file_path}


def handle_delete_fabric(params):
    fabric_index = params["fabric_index"]
    result = fabric_api.DeleteFabric(fabric_index)
    return {"deleted": result, "fabric_index": fabric_index}


# -- Export --

def handle_export_obj(params):
    file_path = params["file_path"]
    options = params.get("options", {})
    if options:
        opt = export_api.NewImportExportOption()
        for key, val in options.items():
            if hasattr(opt, key):
                setattr(opt, key, val)
        result = export_api.ExportOBJ(file_path, opt)
    else:
        result = export_api.ExportOBJ(file_path)
    # ExportOBJ returns list[str], not str
    if isinstance(result, list):
        exported = len(result) > 0
        file_paths = result
    else:
        exported = bool(result)
        file_paths = [result] if result else [file_path]
    return {"exported": exported, "file_paths": file_paths, "format": "obj"}


def handle_export_fbx(params):
    file_path = params["file_path"]
    try:
        options = export_api.ImportExportOption()
        result = export_api.ExportFBX(file_path, options)
    except (AttributeError, TypeError):
        result = export_api.ExportFBX(file_path)
    return {"exported": bool(result), "file_path": result or file_path, "format": "fbx"}


def _build_export_option(options):
    """Build an ImportExportOption from a caller-supplied dict.

    Unknown keys are ignored rather than silently changing behaviour.
    """
    if hasattr(export_api, "ImportExportOption"):
        opt = export_api.ImportExportOption()
    else:
        opt = export_api.NewImportExportOption()
    for key, val in (options or {}).items():
        if hasattr(opt, key):
            setattr(opt, key, val)
    return opt


def handle_export_glb(params):
    file_path = params["file_path"]
    try:
        # was: a fresh ImportExportOption(), discarding params["options"]
        result = export_api.ExportGLB(file_path, _build_export_option(params.get("options")))
    except (AttributeError, TypeError):
        result = export_api.ExportGLB(file_path)
    return {"exported": bool(result), "file_path": result or file_path, "format": "glb"}


def handle_export_gltf(params):
    file_path = params["file_path"]
    try:
        result = export_api.ExportGLTF(
            file_path, _build_export_option(params.get("options")), False)
    except (AttributeError, TypeError):
        result = export_api.ExportGLTF(file_path)
    return {"exported": bool(result), "file_path": result or file_path, "format": "gltf"}


def handle_export_thumbnail(params):
    file_path = params["file_path"]
    result = export_api.ExportThumbnail3D(file_path)
    return {"exported": bool(result), "file_path": result or file_path}


# Camera viewpoints for utility_api.SetCamViewPoint (from CLO's SDK docs)
VIEWPOINTS = {
    "bottom": 0, "three_quarter_right": 1, "front": 2, "three_quarter_left": 3,
    "right": 4, "top": 5, "left": 6, "back": 8,
}


def _snapshot(file_path):
    folder = os.path.dirname(file_path)
    if folder and not os.path.isdir(folder):
        os.makedirs(folder)
    result = export_api.ExportSnapshot3D(file_path)
    return [f for group in (result or []) for f in (group if isinstance(group, (list, tuple)) else [group])]


def handle_export_snapshot(params):
    """Snapshot the 3D view, optionally from one or more camera viewpoints."""
    views = params.get("views") or []
    file_path = params.get("file_path")
    stamp = time.strftime("%Y%m%d_%H%M%S")
    if not views:
        path = file_path or os.path.join(SNAPSHOT_DIR, "snapshot_" + stamp + ".png")
        files = _snapshot(path)
        return {"exported": bool(files), "files": files}

    unknown = [v for v in views if v not in VIEWPOINTS]
    if unknown:
        raise ValueError("Unknown view(s) " + ", ".join(unknown) + "; use " + ", ".join(sorted(VIEWPOINTS)))
    base = os.path.splitext(file_path)[0] if file_path else os.path.join(SNAPSHOT_DIR, "snapshot_" + stamp)
    files = {}
    for view in views:
        utility_api.SetCamViewPoint(VIEWPOINTS[view])
        files[view] = _snapshot(base + "_" + view + ".png")
    utility_api.SetCamViewPoint(VIEWPOINTS["front"])
    return {"exported": all(files.values()), "files": files}


def handle_export_turntable(params):
    file_path = params["file_path"]
    num_images = params.get("number_of_images", 36)
    width = params.get("width", 2500)
    height = params.get("height", 2500)
    result = export_api.ExportTurntableImages(file_path, num_images, width, height)
    return {"exported": bool(result), "file_path": result or file_path,
            "number_of_images": num_images, "width": width, "height": height}


def handle_export_tech_pack(params):
    file_path = params["file_path"]
    # CLO3D 2024.1 declares ExportTechPack(path, ExportTechpackOption) but exposes no ExportTechpackOption to
    # Python, so that call cannot be made (seen live 2026-10-15). ExportTechPackToStream returns the same tech
    # pack as JSON, and we write it. The file's existence is checked, never assumed.
    route = "ExportTechPack"
    option_cls = getattr(export_api, "ExportTechpackOption", None)
    if option_cls is not None:
        export_api.ExportTechPack(file_path, option_cls())
    else:
        route = "ExportTechPackToStream"
        data = export_api.ExportTechPackToStream(file_path)
        if not data:
            raise RuntimeError("CLO3D returned an empty tech pack")
        folder = os.path.dirname(file_path)
        if folder and not os.path.isdir(folder):
            os.makedirs(folder)
        with open(file_path, "w", encoding="utf-8") as f:
            f.write(data)
    if not os.path.isfile(file_path):
        raise RuntimeError("CLO3D reported success but wrote no file at " + file_path)
    return {"exported": True, "file_path": file_path, "bytes": os.path.getsize(file_path), "route": route}


# -- Import --

def handle_import_file(params):
    file_path = params["file_path"]
    result = import_api.ImportFile(file_path)
    return {"imported": result, "file_path": file_path}


DXF_DEFAULTS = {"m_bAppend": True, "m_bIncludeNotchs": True, "m_bImportPatternAnnotation": True}


def handle_import_dxf(params):
    """Import a DXF(-AAMA) pattern through CLO's own DXF importer with its option record. CLO's defaults leave
    append OFF (the import replaces the open patterns) and notches OFF; a block is opened to design from, so this
    appends and keeps notches unless the caller says otherwise. Option names are ApiTypes.ImportDxfOption's own,
    read from CLO3D 2024.1 on 2026-10-16."""
    import ApiTypes
    file_path = params["file_path"]
    opt = ApiTypes.ImportDxfOption()
    for k, v in {**DXF_DEFAULTS, **(params.get("options") or {})}.items():
        if not hasattr(opt, k):
            raise ValueError("ImportDxfOption has no field " + k)
        setattr(opt, k, v)
    ok = bool(import_api.ImportDXF(file_path, opt))
    return {"imported": ok, "file_path": file_path, "route": "ImportDXF",
            "options": {k: getattr(opt, k) for k in dir(opt) if k.startswith("m_")}}


def handle_import_avatar(params):
    file_path = params["file_path"]
    apf_path = params.get("apf_path", "")
    # ImportAVAC takes CLO3D's .avac only; an .avt (CLO3D's own avatar files) returns False from it
    # and loads through ImportFile (seen live in CLO3D 2024.1, 2026-10-16).
    if file_path.lower().endswith(".avac"):
        result = import_api.ImportAVAC(file_path, apf_path)
        route = "ImportAVAC"
    else:
        result = import_api.ImportFile(file_path)
        route = "ImportFile"
    return {"imported": bool(result), "file_path": file_path, "route": route}


def handle_import_fabric(params):
    file_path = params["file_path"]
    index = fabric_api.AddFabric(file_path)
    return {"imported": True, "fabric_index": index, "file_path": file_path}


# -- Simulation --

def handle_simulate(params):
    steps = params.get("steps", 100)
    ok = utility_api.Simulate(steps)
    return {"simulated": bool(ok), "steps": steps}


def handle_set_simulation_quality(params):
    # SDK: SetSimulationQuality(int quality, int simulationMode) - both required.
    # quality 0=Normal 1=Animation 2=Fitting 3=FAST(GPU); mode 0=CPU 1=FAST(GPU)
    quality = params["quality"]
    simulation_mode = params.get("simulation_mode", 0)
    utility_api.SetSimulationQuality(quality, simulation_mode)
    return {"quality": quality, "simulation_mode": simulation_mode}


def handle_get_colorways(params):
    count = utility_api.GetColorwayCount()
    names = export_api.GetColorwayNameList()
    current = utility_api.GetCurrentColorwayIndex()
    colorways = []
    if names:
        for i, name in enumerate(names):
            colorways.append({"index": i, "name": name, "current": i == current})
    else:
        for i in range(count):
            colorways.append({"index": i, "current": i == current})
    return {"colorways": colorways, "count": count, "current_index": current}


def handle_set_current_colorway(params):
    index = params["colorway_index"]
    utility_api.SetCurrentColorwayIndex(index)
    return {"set": True, "colorway_index": index}


def handle_set_colorway_name(params):
    index = params["colorway_index"]
    name = params["name"]
    utility_api.SetColorwayName(index, name)
    return {"set": True, "colorway_index": index, "name": name}


def handle_copy_colorway(params):
    # SDK: CopyColorway(index, copyOption) -> index of the new colorway.
    # copyOption 0=unlink all 1=unlink materials only 2=link all
    index = params["colorway_index"]
    copy_option = params.get("copy_option", 0)
    new_index = utility_api.CopyColorway(index, copy_option)
    return {"copied": True, "source_index": index,
            "copy_option": copy_option, "new_index": new_index}


def handle_delete_colorway(params):
    index = params["colorway_index"]
    utility_api.DeleteColorwayItem(index)
    return {"deleted": True, "colorway_index": index}


# -- Avatar --

def handle_get_avatars(params):
    count = export_api.GetAvatarCount()
    names = export_api.GetAvatarNameList()
    genders = export_api.GetAvatarGenderList()
    avatars = []
    if names:
        for i, name in enumerate(names):
            avatar = {"index": i, "name": name}
            if genders and i < len(genders):
                avatar["gender"] = genders[i]
            avatars.append(avatar)
    return {"avatars": avatars, "count": count}


def handle_show_hide_avatar(params):
    show = params.get("show", True)
    utility_api.SetShowHideAvatar(show)
    return {"visible": show}


def handle_get_avatar_genders(params):
    genders = export_api.GetAvatarGenderList()
    return {"genders": genders or []}


# ---------------------------------------------------------------------------
# Avatar, arrangement and asset library
# ---------------------------------------------------------------------------

def _library_roots(kind):
    """CLO asset folders for a kind ("Avatar", "Fabric"): user library first, then the app bundle."""
    return [
        os.path.join(ASSET_DIR, kind),
        os.path.join("/Applications/CLO.app/Contents/Assets/CLO Assets", kind),
    ]


def _library_files(kind, extension):
    """Map file name -> path for every library file of a kind, user library taking precedence."""
    found = {}
    for root in _library_roots(kind):
        for folder, _dirs, files in os.walk(root):
            for name in files:
                if name.lower().endswith(extension) and name not in found:
                    found[name] = os.path.join(folder, name)
    return found


def handle_list_library(params):
    kind = params["kind"]
    extensions = {"Avatar": ".avt", "Fabric": ".zfab"}
    if kind not in extensions:
        raise ValueError("kind must be one of " + ", ".join(extensions))
    extension = extensions[kind]
    contains = (params.get("contains") or "").lower()
    files = _library_files(kind, extension)
    matching = [{"name": name, "path": path} for name, path in files.items() if contains in name.lower()]
    return {"files": sorted(matching, key=lambda f: f["name"])}


def handle_load_avatar(params):
    """Load an avatar by library name (e.g. "Luka") or by .avt path."""
    avatar = params["avatar"]
    path = avatar
    if not os.path.isfile(path):
        matches = [p for name, p in _library_files("Avatar", ".avt").items() if avatar.lower() in name.lower()]
        if not matches:
            raise ValueError("No avatar matching " + repr(avatar) + " in the CLO asset library")
        if len(matches) > 1:
            raise ValueError("Several avatars match " + repr(avatar) + ": " + ", ".join(sorted(os.path.basename(m) for m in matches)))
        path = matches[0]
    if not import_api.ImportFile(path):
        raise RuntimeError("CLO3D could not load avatar " + path)
    return {"loaded": path, "arrangement_points": len(pattern_api.GetArrangementList())}


def _arrangement_points():
    return [
        {"index": int(p["ArrangementIndex"]), "name": p["ArrangementName"]}
        for p in pattern_api.GetArrangementList()
    ]


def handle_list_arrangement_points(params):
    contains = (params.get("contains") or "").lower()
    points = [p for p in _arrangement_points() if contains in p["name"].lower()]
    if not points and not contains:
        raise RuntimeError("No arrangement points. Load an avatar first.")
    return {"points": points}


def handle_arrange_pattern(params):
    """Place a pattern on an avatar arrangement point, given by name or index."""
    idx = params["pattern_index"]
    point = params["point"]
    points = _arrangement_points()
    if isinstance(point, str) and not point.isdigit():
        match = [p for p in points if p["name"].lower() == point.lower()]
        if not match:
            raise ValueError("Unknown arrangement point " + repr(point) + ". Use list_arrangement_points.")
        point = match[0]["index"]
    pattern_api.SetArrangement(idx, int(point))
    if params.get("orientation") is not None:
        pattern_api.SetArrangementOrientation(idx, int(params["orientation"]))
    return {"arrangement": pattern_api.GetArrangementOfPattern(idx)}


def handle_reset_garment(params):
    """Put pieces back in their arranged positions, undoing simulation."""
    utility_api.ResetClothArrangement()
    return {"reset": True}


# ---------------------------------------------------------------------------
# Drafting
# ---------------------------------------------------------------------------

def _line_info(pattern_index):
    """Outline lines of a pattern as parsed JSON (line i runs from point i to i+1)."""
    return json.loads(pattern_api.GetPatternLineInfo(pattern_index))


def handle_get_pattern_lines(params):
    return _line_info(params["pattern_index"])


def handle_sew_patterns(params):
    a, b = params["pattern_a"], params["pattern_b"]
    dir_a, dir_b = params.get("direction_a", True), params.get("direction_b", True)
    child_b = params.get("internal_shape_b")
    before = set(pattern_api.GetSeamlinePairGroupListInPattern(a))
    if child_b is None:
        pattern_api.AddSeamlinePairGroup(a, params["line_a"], b, params["line_b"], dir_a, dir_b)
    else:
        # Sew to a line of an internal shape on pattern B (e.g. a pocket placement line)
        pattern_api.AddSeamlinePairGroup(a, params["line_a"], b, child_b, params["line_b"], dir_a, dir_b)
    # CLO returns True even for invalid indices, so check a seam actually appeared
    sewn = bool(set(pattern_api.GetSeamlinePairGroupListInPattern(a)) - before)
    if not sewn:
        raise RuntimeError("CLO3D did not create the seam; check the pattern, line and internal shape indices")
    return {"sewn": True, "seam_count": pattern_api.GetSeamlinePairGroupCount()}


def handle_unfold_pattern(params):
    idx = params["pattern_index"]
    unfolded = pattern_api.UnfoldPatternPiece(idx, params["line_index"], params.get("half_symmetry", False))
    # Unfolding renumbers the outline lines, so return the new numbering.
    return {"unfolded": bool(unfolded), "lines": _line_info(idx)}


def handle_move_pattern(params):
    idx = params["pattern_index"]
    pattern_api.SetPatternPieceMove(idx, params["dx"], params["dy"])
    return {"position": list(pattern_api.GetPatternPiecePos(idx))}


def handle_set_grain_direction(params):
    idx = params["pattern_index"]
    pattern_api.SetPatternPieceGrainDirection(idx, params["degrees"])
    return {"degrees": pattern_api.GetPatternPieceGrainDirection(idx)}


def handle_add_internal_line(params):
    points = [(float(p[0]), float(p[1]), int(p[2]) if len(p) > 2 else 0) for p in params["points"]]
    child = pattern_api.CreateInternalShapeWithPoints(params["pattern_index"], points, params.get("closed", False))
    return {"internal_shape_index": child}


# ---------------------------------------------------------------------------
# Generic API access
# ---------------------------------------------------------------------------

API_MODULE_NAMES = ["export_api", "fabric_api", "import_api", "pattern_api", "utility_api", "rest_api"]


def _api_modules():
    """Return the CLO3D API modules importable in this session, by name."""
    modules = {}
    for name in API_MODULE_NAMES:
        try:
            modules[name] = __import__(name)
        except ImportError:
            pass
    return modules


def _jsonable(value):
    """Convert an API return value into something json.dumps accepts."""
    if value is None or isinstance(value, (bool, int, float, str)):
        return value
    if isinstance(value, (list, tuple, set)):
        return [_jsonable(v) for v in value]
    if isinstance(value, dict):
        return {str(k): _jsonable(v) for k, v in value.items()}
    return repr(value)


def handle_list_api(params):
    modules = _api_modules()
    wanted = params.get("module")
    contains = (params.get("contains") or "").lower()
    result = {}
    for name, module in modules.items():
        if wanted and name != wanted:
            continue
        functions = sorted(
            attr for attr in dir(module)
            if not attr.startswith("_") and callable(getattr(module, attr))
            and contains in attr.lower()
        )
        if params.get("with_docs"):
            # pybind11 docstrings carry the exact Python signature
            functions = {f: (getattr(module, f).__doc__ or "").strip() for f in functions}
        result[name] = functions
    return {"modules": result}


def handle_call_api(params):
    modules = _api_modules()
    module_name = params["module"]
    if module_name not in modules:
        raise ValueError("Unknown module " + module_name + "; available: " + ", ".join(sorted(modules)))
    func = getattr(modules[module_name], params["function"], None)
    if func is None or not callable(func):
        raise ValueError(module_name + " has no function " + params["function"])

    args = params.get("args") or []
    if params.get("inner_lists_as_tuples"):
        # e.g. CreatePatternWithPoints wants a list of (x, y, type) tuples
        args = [
            [tuple(item) if isinstance(item, list) else item for item in arg] if isinstance(arg, list) else arg
            for arg in args
        ]
    return {"result": _jsonable(func(*args))}


def handle_reload_plugin(params):
    """Reload this module from disk in place; the native pump keeps calling poll_once on it.
    Registered only under the pump (dev mode has its own reload, which re-runs the file)."""
    module = sys.modules.get(__name__)
    if __name__ == "__main__" or module is None:
        raise RuntimeError("Reload only works when the native pump loaded the plugin")
    import importlib
    importlib.reload(module)
    return {"reloaded": True, "handlers": len(module.HANDLERS)}


# ---------------------------------------------------------------------------
# Handler registry
# ---------------------------------------------------------------------------

def handle_stop(params):
    state = getattr(builtins, "_clo3d_mcp", None)
    if state:
        state["stop"] = True
    return {"stopped": True}


HANDLERS = {
    "ping": handle_ping,
    "stop": handle_stop,
    "get_project_info": handle_get_project_info,
    "new_project": handle_new_project,
    "open_file": handle_open_file,
    "save_file": handle_save_file,
    "get_garment_info": handle_get_garment_info,
    "get_pattern_count": handle_get_pattern_count,
    "get_pattern_list": handle_get_pattern_list,
    "get_pattern_info": handle_get_pattern_info,
    "get_bounding_box": handle_get_bounding_box,
    "set_pattern_name": handle_set_pattern_name,
    "copy_pattern": handle_copy_pattern,
    "delete_pattern": handle_delete_pattern,
    "flip_pattern": handle_flip_pattern,
    "create_pattern": handle_create_pattern,
    "get_arrangement_list": handle_get_arrangement_list,
    "get_fabric_count": handle_get_fabric_count,
    "get_fabric_list": handle_get_fabric_list,
    "add_fabric": handle_add_fabric,
    "assign_fabric": handle_assign_fabric,
    "set_fabric_color": handle_set_fabric_color,
    "get_fabric_for_pattern": handle_get_fabric_for_pattern,
    "replace_fabric": handle_replace_fabric,
    "delete_fabric": handle_delete_fabric,
    "export_obj": handle_export_obj,
    "export_fbx": handle_export_fbx,
    "export_glb": handle_export_glb,
    "export_gltf": handle_export_gltf,
    "export_thumbnail": handle_export_thumbnail,
    "export_snapshot": handle_export_snapshot,
    "export_turntable": handle_export_turntable,
    "export_tech_pack": handle_export_tech_pack,
    "import_file": handle_import_file,
    "import_dxf": handle_import_dxf,
    "import_avatar": handle_import_avatar,
    "import_fabric": handle_import_fabric,
    "simulate": handle_simulate,
    "set_simulation_quality": handle_set_simulation_quality,
    "get_colorways": handle_get_colorways,
    "set_current_colorway": handle_set_current_colorway,
    "set_colorway_name": handle_set_colorway_name,
    "copy_colorway": handle_copy_colorway,
    "delete_colorway": handle_delete_colorway,
    "get_avatars": handle_get_avatars,
    "show_hide_avatar": handle_show_hide_avatar,
    "get_avatar_genders": handle_get_avatar_genders,
    "list_library": handle_list_library,
    "load_avatar": handle_load_avatar,
    "list_arrangement_points": handle_list_arrangement_points,
    "arrange_pattern": handle_arrange_pattern,
    "reset_garment": handle_reset_garment,
    "get_pattern_lines": handle_get_pattern_lines,
    "sew_patterns": handle_sew_patterns,
    "unfold_pattern": handle_unfold_pattern,
    "move_pattern": handle_move_pattern,
    "set_grain_direction": handle_set_grain_direction,
    "add_internal_line": handle_add_internal_line,
    "list_api": handle_list_api,
    "call_api": handle_call_api,
}

if getattr(builtins, "_CLO3D_MCP_NATIVE_PUMP", False):
    HANDLERS["reload_plugin"] = handle_reload_plugin


# ---------------------------------------------------------------------------
# File-based communication
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Dev mode: run test code inside CLO3D and reload this file, without the Script Editor.
# Off unless the launcher sets CLO3D_MCP_DEV before running this file, because run_script executes
# whatever code arrives in the request directory. Started by clo3d_mcp_dev.py (see README, Development).
# ---------------------------------------------------------------------------

DEV = bool(globals().get("CLO3D_MCP_DEV") or os.environ.get("CLO3D_MCP_DEV"))
PLUGIN_PATH = globals().get("CLO3D_MCP_PLUGIN_PATH") or globals().get("__file__")


def handle_run_script(params):
    """Run Python inside CLO3D (code, or a file path) and return what it printed, or the error."""
    import io
    import traceback
    from contextlib import redirect_stdout, redirect_stderr

    code = params.get("code")
    if code is None:
        with open(params["path"], encoding="utf-8") as f:
            code = f.read()
    ns = {"__name__": "__clo3d_mcp_script__"}
    for api in ("export_api", "fabric_api", "import_api", "pattern_api", "utility_api"):
        if api in globals():
            ns[api] = globals()[api]
    out = io.StringIO()
    try:
        with redirect_stdout(out), redirect_stderr(out):
            exec(compile(code, params.get("path") or "<run_script>", "exec"), ns)
        return {"ok": True, "output": out.getvalue(), "result": repr(ns.get("result")) if "result" in ns else None}
    except Exception:
        return {"ok": False, "output": out.getvalue(), "error": traceback.format_exc()}


def handle_dev_reload_plugin(params):
    """Re-run this plug-in file from disk; the new listener replaces this one."""
    path = params.get("path") or PLUGIN_PATH
    if not path or not os.path.isfile(path):
        raise ValueError("no plug-in file to reload: " + str(path))
    with open(path, encoding="utf-8") as f:
        src = f.read()
    compile(src, path, "exec")  # a syntax error is refused here and the running listener stays up
    retired = getattr(builtins, "_clo3d_mcp_retired", [])
    retired.append(getattr(builtins, "_clo3d_mcp", None))  # keep the running callback alive until it returns
    builtins._clo3d_mcp_retired = retired[-8:]
    ns = {"__name__": "__main__", "CLO3D_MCP_DEV": True, "CLO3D_MCP_PLUGIN_PATH": path}
    exec(compile(src, path, "exec"), ns)
    return {"reloaded": path}


def process_command(data):
    """Parse and execute a single JSON command."""
    try:
        request = json.loads(data)
    except (json.JSONDecodeError, ValueError) as e:
        return json.dumps({"id": None, "status": "error", "message": "Invalid JSON: " + str(e)})

    req_id = request.get("id")
    cmd_type = request.get("type")
    params = request.get("params", {})

    handler = HANDLERS.get(cmd_type)
    if not handler and DEV:
        handler = {"run_script": handle_run_script, "reload_plugin": handle_dev_reload_plugin}.get(cmd_type)
    if not handler:
        return json.dumps({
            "id": req_id,
            "status": "error",
            "message": "Unknown command: " + str(cmd_type),
        })

    try:
        result = handler(params)
        return json.dumps({"id": req_id, "status": "success", "result": result})
    except Exception as e:
        return json.dumps({
            "id": req_id,
            "status": "error",
            "message": str(type(e).__name__) + ": " + str(e),
        })


# ---------------------------------------------------------------------------
# Listener
#
# CLO3D's embedded Python does not schedule background threads once the
# Script Editor run returns, so a threading.Thread poller never runs (seen on
# macOS and on Windows). On Windows, a native Win32 timer is dispatched by
# CLO3D's own UI event loop on the main thread: each tick handles at most one
# request and returns, so CLO3D stays responsive. On other platforms, fall
# back to a blocking loop (the Script Editor stays busy while listening).
# State lives in `builtins` so that re-running the script replaces the
# previous listener and the timer callback is never garbage-collected.
# ---------------------------------------------------------------------------

def _import_ctypes():
    """ctypes, finding CLO3D's own copy if it is not on the path.

    CLO3D ships _ctypes.pyd in a PythonLib folder beside python37.dll but does not put that folder on
    sys.path, so a bare `import ctypes` fails with "No module named '_ctypes'" (seen in CLO3D
    Standalone OnlineAuth, Python 3.7). Look beside the executable and beside the bundled
    python3x.zip, add the folder that holds _ctypes, and import again. Returns None if none does.
    """
    try:
        import ctypes  # noqa: F401
        return ctypes
    except ImportError:
        pass
    roots = [os.path.dirname(sys.executable)]
    roots += [os.path.dirname(p) for p in sys.path if p.lower().endswith(".zip")]
    for root in roots:
        for sub in ("PythonLib", "DLLs", ""):
            folder = os.path.join(root, sub) if sub else root
            if os.path.isfile(os.path.join(folder, "_ctypes.pyd")) and folder not in sys.path:
                sys.path.append(folder)
                try:
                    import ctypes  # noqa: F811
                    print("[CLO MCP] Found ctypes in " + folder)
                    return ctypes
                except ImportError:
                    sys.path.remove(folder)
    return None


_HAVE_TIMER = False
if sys.platform == "win32":
    ctypes = _import_ctypes()
    _HAVE_TIMER = ctypes is not None

if _HAVE_TIMER:
    import ctypes.wintypes as _wt

    _user32 = ctypes.windll.user32
    _TIMERPROC = ctypes.WINFUNCTYPE(None, _wt.HWND, _wt.UINT, ctypes.c_size_t, _wt.DWORD)
    _user32.SetTimer.argtypes = [_wt.HWND, ctypes.c_size_t, _wt.UINT, _TIMERPROC]
    _user32.SetTimer.restype = ctypes.c_size_t
    _user32.KillTimer.argtypes = [_wt.HWND, ctypes.c_size_t]
    _user32.KillTimer.restype = _wt.BOOL


def prepare_comm_dir():
    """Create the comm directory and remove stale files from a previous run."""
    if not os.path.exists(COMM_DIR):
        os.makedirs(COMM_DIR)

    for name in os.listdir(COMM_DIR):
        if name.startswith(("req_", "resp_")) or name in ("request.json", "response.json", "stop"):
            try:
                os.remove(os.path.join(COMM_DIR, name))
            except OSError:
                pass

    print("[CLO MCP] Listening for commands in: " + COMM_DIR)


def _pending_requests():
    """Complete request files (req_<id>.json), oldest first."""
    paths = [
        os.path.join(COMM_DIR, name) for name in os.listdir(COMM_DIR)
        if name.startswith("req_") and name.endswith(".json")
    ]
    return sorted(paths, key=lambda path: os.path.getmtime(path) if os.path.exists(path) else 0)


def poll_once():
    """Handle every pending request file.

    Each request req_<id>.json gets its own response resp_<id>.json, so
    several MCP servers can share the directory without clobbering each other.
    """
    try:
        requests = _pending_requests()
    except OSError as e:
        print("[CLO MCP] Error: " + str(e))
        return

    for request_file in requests:
        try:
            with open(request_file, "r") as f:
                data = f.read()
            try:
                os.remove(request_file)
            except OSError:
                pass

            req_id = os.path.basename(request_file)[len("req_"):-len(".json")]
            response = process_command(data)

            # Write to temp file first, then rename (atomic)
            response_file = os.path.join(COMM_DIR, "resp_" + req_id + ".json")
            tmp_file = response_file + ".tmp"
            with open(tmp_file, "w") as f:
                f.write(response)
            os.rename(tmp_file, response_file)

        except Exception as e:
            print("[CLO MCP] Error: " + str(e))


def _tick(hwnd, msg, timer_id, time_ms):
    state = getattr(builtins, "_clo3d_mcp", None)
    if not state or state["busy"]:  # a long call (e.g. simulate) may pump messages and re-enter
        return
    state["busy"] = True
    try:
        poll_once()
    except Exception as e:
        print("[CLO MCP] Error: " + str(e))
    finally:
        state["busy"] = False
    if state["stop"] and getattr(builtins, "_clo3d_mcp", None) is state:
        stop()


def start():
    """Start listening. Re-running the script replaces the previous listener."""
    if getattr(builtins, "_CLO3D_MCP_NATIVE_PUMP", False):
        # The native pump (native/clo3d_mcp_pump.cpp) imported this module and calls
        # poll_once() from CLO3D's main thread itself.
        return
    stop(quiet=True)
    prepare_comm_dir()
    builtins._clo3d_mcp = {"timer_id": None, "callback": None, "busy": False, "stop": False}
    state = builtins._clo3d_mcp

    if sys.platform == "win32" and not _HAVE_TIMER:
        # A blocking loop would freeze CLO3D's whole window, so refuse rather than fall back to it.
        print("[CLO MCP] Not started: ctypes is not available in CLO3D's Python, so the plug-in cannot")
        print("[CLO MCP] listen without freezing CLO3D. Looked beside " + sys.executable + " and its python3x.zip.")
        print("[CLO MCP] Please report this at github.com/Ubani-Studio/clo3d-mcp/issues with your CLO3D version.")
        return

    if sys.platform == "win32":
        callback = _TIMERPROC(_tick)
        timer_id = _user32.SetTimer(None, 0, int(POLL_INTERVAL * 1000), callback)
        if timer_id:
            state["timer_id"], state["callback"] = timer_id, callback
            print("[CLO MCP] Plugin started")
            print("[CLO MCP] CLO3D stays usable while listening.")
            return
        print("[CLO MCP] Could not create a Windows timer, falling back to a blocking loop.")

    print("[CLO MCP] Plugin started (the Script Editor stays busy while listening)")
    while not state["stop"]:
        try:
            poll_once()
        except Exception as e:
            print("[CLO MCP] Error: " + str(e))
        time.sleep(POLL_INTERVAL)
    print("[CLO MCP] Plugin stopped")


def stop(quiet=False):
    """Stop listening (also reachable through the "stop" command)."""
    state = getattr(builtins, "_clo3d_mcp", None)
    if not state:
        return
    state["stop"] = True
    timer_id = state.get("timer_id") or state.get("id")  # "id": earlier local builds of this listener
    if timer_id and _HAVE_TIMER:
        _user32.KillTimer(None, timer_id)
        state["timer_id"] = state["id"] = None
        if not quiet:
            print("[CLO MCP] Plugin stopped")


# Auto-start
if __name__ == "__main__" or IN_CLO3D:
    start()

