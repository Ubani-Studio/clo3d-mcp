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
COMM_DIR = os.environ.get("CLO3D_MCP_DIR") or os.path.join(
    tempfile.gettempdir(), "clo3d_mcp"
)
REQUEST_FILE = os.path.join(COMM_DIR, "request.json")
RESPONSE_FILE = os.path.join(COMM_DIR, "response.json")
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


def handle_export_snapshot(params):
    file_path = params["file_path"]
    result = export_api.ExportSnapshot3D(file_path)
    return {"exported": bool(result), "file_path": result or file_path}


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
    utility_api.Simulate(steps)
    return {"simulated": True, "steps": steps}


def handle_set_simulation_quality(params):
    # SDK: SetSimulationQuality(int quality, int simulationMode) - both required.
    # quality 0=Normal 1=Animation 2=Fitting 3=FAST(GPU); mode 0=CPU 1=FAST(GPU)
    quality = params["quality"]
    simulation_mode = params.get("simulation_mode", 0)
    utility_api.SetSimulationQuality(quality, simulation_mode)
    return {"quality": quality, "simulation_mode": simulation_mode}


# -- Colorway --

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
# EXTENDED (clo3d-mcp-extended): seams, internal lines, arrangement, piece state, graphics, fabrics,
# trims, grading, visuals, nesting, API docs, whole garment in one call
# ---------------------------------------------------------------------------
def _points(points):
    # CLO's nested casters reject ints where floats are expected: convert explicitly.
    return [(float(p[0]), float(p[1]), int(p[2]) if len(p) > 2 else 0) for p in points]


def handle_get_line_lengths(params):
    index = int(params["pattern_index"])
    count = int(params["count"])
    return {"index": index, "lengths": [pattern_api.GetLineLength(index, i) for i in range(count)]}


def handle_sew(params):
    # Overloads: outline-outline, outline-internal shape (child_b), internal-internal (child_a and child_b).
    pa, la, pb, lb = int(params["pattern_a"]), int(params["line_a"]), int(params["pattern_b"]), int(params["line_b"])
    da, db = bool(params.get("direction_a", True)), bool(params.get("direction_b", True))
    child_a, child_b = params.get("child_a"), params.get("child_b")
    if child_a is not None and child_b is not None:
        result = pattern_api.AddSeamlinePairGroup(pa, int(child_a), la, pb, int(child_b), lb, da, db)
    elif child_b is not None:
        result = pattern_api.AddSeamlinePairGroup(pa, la, pb, int(child_b), lb, da, db)
    else:
        result = pattern_api.AddSeamlinePairGroup(pa, la, pb, lb, da, db)
    return {"result": str(result)}


def handle_get_seam_groups(params):
    count = pattern_api.GetSeamlinePairGroupCount()
    groups = [{"index": i, "name": pattern_api.GetSeamlinePairGroupName(i)} for i in range(count)]
    out = {"count": count, "groups": groups}
    if params.get("pattern_index") is not None:
        out["in_pattern"] = _items(pattern_api.GetSeamlinePairGroupListInPattern(int(params["pattern_index"])))
    return out


# -- Arrangement on the avatar --

def handle_arrange_pattern(params):
    _require_avatar()
    index = int(params["pattern_index"])
    done = []
    if params.get("arrangement_index") is not None:
        pattern_api.SetArrangement(index, int(params["arrangement_index"]))
        done.append("arrangement")
    if params.get("x") is not None and params.get("y") is not None:
        pattern_api.SetArrangementPosition(index, int(params["x"]), int(params["y"]), int(params.get("offset", 0)))
        done.append("position")
    if params.get("orientation") is not None:
        pattern_api.SetArrangementOrientation(index, int(params["orientation"]))
        done.append("orientation")
    if params.get("shape_style") is not None:
        pattern_api.SetArrangementShapeStyle(index, str(params["shape_style"]))
        done.append("shape_style")
    return {"pattern_index": index, "applied": done}


def handle_get_pattern_arrangements(params):
    return {"arrangements": [dict(a) if isinstance(a, dict) else _fields(a) for a in _items(pattern_api.GetArrangementOfPattern())]}


def handle_reset_cloth_arrangement(params):
    utility_api.ResetClothArrangement()
    return {"reset": True}


# -- Internal and base shapes --

def handle_create_internal_shape(params):
    index = int(params["pattern_index"])
    closed = bool(params.get("closed", False))
    if params.get("base", False):
        child = pattern_api.CreateBaseShapeWithPoints(index, _points(params["points"]), closed)
    else:
        child = pattern_api.CreateInternalShapeWithPoints(index, _points(params["points"]), closed)
    return {"pattern_index": index, "child_index": child}


def handle_convert_shape(params):
    index, child = int(params["pattern_index"]), int(params["child_index"])
    if params.get("to", "internal") == "base":
        pattern_api.ConvertToBaseLine(index, child)
    else:
        pattern_api.ConvertToInternalLine(index, child)
    return {"pattern_index": index, "child_index": child, "to": params.get("to", "internal")}


def handle_offset_as_internal_line(params):
    pattern_api.OffsetAsInternalLine(int(params["pattern_index"]), int(params["line_index"]),
                                     int(params.get("number", 1)), float(params["distance"]),
                                     bool(params.get("reverse", False)), bool(params.get("extend", False)))
    return {"offset": True}


def handle_delete_line(params):
    pattern_api.DeleteLine(int(params["pattern_index"]), int(params["line_index"]))
    return {"deleted": True}


def handle_delete_point(params):
    pattern_api.DeletePoint(int(params["pattern_index"]), int(params["point_index"]))
    return {"deleted": True}


# -- Pattern piece properties --

def _safe(function, *args):
    """Call a read-only getter; an unavailable value must not hide the others."""
    try:
        value = function(*args)
    except Exception as e:
        return "error: " + str(e)
    if isinstance(value, (list, tuple)):
        return [v if isinstance(v, (bool, int, float, str, list, dict)) else _fields(v) for v in value]
    if isinstance(value, (bool, int, float, str, dict)) or value is None:
        return value
    return _fields(value)


def _json_or_text(text):
    try:
        return json.loads(text)
    except (TypeError, ValueError):
        return text


def handle_get_pattern_details(params):
    index = int(params["pattern_index"])
    return {"pattern_index": index,
            "position": _safe(pattern_api.GetPatternPiecePos, index),
            "area": _safe(pattern_api.GetPatternPieceArea, index),
            "grain_direction": _safe(pattern_api.GetPatternPieceGrainDirection, index),
            "category": _safe(pattern_api.GetPatternPieceCategory, index),
            "classification": _safe(pattern_api.GetPatternPieceClassification, index),
            "layer": _safe(pattern_api.GetPatternLayer, index),
            "fabric_index": _safe(pattern_api.GetPatternPieceFabricIndex, index),
            "particle_distance": _safe(pattern_api.GetParticleDistanceOfPattern, index),
            "shrinkage": _safe(pattern_api.GetShrinkagePercentage, index),
            "solidify": _safe(pattern_api.IsPatternPieceSolidify, index),
            "solidify_strengthen": _safe(pattern_api.GetPatternPieceSolidifyStrengthen, index),
            "grain_link_all_colorways": _safe(pattern_api.IsPatternPieceGrainLinkAllColorways, index),
            "linked_patterns": _safe(pattern_api.GetLinkedPatternIndex, index)}


def handle_set_pattern_position(params):
    pattern_api.SetPatternPiecePos(int(params["pattern_index"]), float(params["x"]), float(params["y"]))
    return {"moved": True}


def handle_set_grain_direction(params):
    pattern_api.SetPatternPieceGrainDirection(int(params["pattern_index"]), float(params["degrees"]))
    return {"grain_direction": float(params["degrees"])}


def handle_set_particle_distance(params):
    pattern_api.SetParticleDistanceOfPattern(int(params["pattern_index"]), float(params["distance"]))
    return {"particle_distance": float(params["distance"])}


def handle_unfold_pattern(params):
    pattern_api.UnfoldPatternPiece(int(params["pattern_index"]), int(params["line_index"]),
                                   bool(params.get("half_symmetry", False)))
    return {"unfolded": True}


def handle_set_shirring(params):
    index, line = int(params["pattern_index"]), int(params.get("line_index", -1))
    pattern_api.SetPatternPieceShirring(index, line, bool(params.get("enabled", True)))
    if params.get("height") is not None:
        pattern_api.SetPatternPieceShirringHeight(index, line, float(params["height"]))
    if params.get("interval") is not None:
        pattern_api.SetPatternPieceShirringInterval(index, line, float(params["interval"]))
    if params.get("extend") is not None:
        pattern_api.SetPatternPieceShirringExtend(index, line, bool(params["extend"]))
    return {"pattern_index": index, "line_index": line}


def handle_set_elastic(params):
    index, line = int(params["pattern_index"]), int(params.get("line_index", -1))
    pattern_api.SetPatternPieceElastic(index, line, bool(params.get("enabled", True)))
    if params.get("strength") is not None:
        pattern_api.SetPatternPieceElasticStrength(index, line, float(params["strength"]))
    if params.get("strength_ratio") is not None:
        pattern_api.SetPatternPieceElasticStrengthRatio(index, line, int(params["strength_ratio"]))
    if params.get("total_length") is not None:
        pattern_api.SetPatternPieceElasticTotalLength(index, line, float(params["total_length"]))
    if params.get("segment_length") is not None:
        pattern_api.SetPatternPieceElasticSegmentLength(index, line, float(params["segment_length"]))
    return {"pattern_index": index, "line_index": line}


# -- Pattern JSON, measurements, points of measure --

def handle_export_pattern_json(params):
    return {"exported": bool(pattern_api.ExportPatternJSON(str(params["file_path"]))), "file_path": params["file_path"]}


def handle_import_pattern_json(params):
    return {"imported": bool(pattern_api.ImportPatternJSON(str(params["file_path"])))}


def handle_import_avatar_measurements(params):
    return {"imported": bool(import_api.ImportMeasurement(str(params["csv_path"])))}


def handle_export_pom(params):
    return {"file_path": export_api.ExportPOM()}


# -- Views, images and fit maps --

def handle_set_camera_view(params):
    utility_api.SetCamViewPoint(int(params["view_index"]))
    if params.get("zoom_to_garment", False):
        utility_api.SetZoomView()
    utility_api.Refresh3DWindow()
    return {"view_index": int(params["view_index"])}


def _image_path(target, default_name):
    # CLO expects a file path with an extension, even where the SDK documents a folder.
    target = str(target)
    if os.path.splitext(target)[1]:
        return target
    return os.path.join(target, default_name)


def handle_export_view_image(params):
    view = int(params["view_index"])
    path = export_api.ExportStdViewImage(view, _image_path(params["output_path"], "view_%d.png" % view),
                                         int(params.get("colorway_index", 0)),
                                         int(params.get("width", 1024)), int(params.get("height", 1024)))
    return {"result": path}


def handle_export_custom_view_snapshot(params):
    paths = export_api.ExportCustomViewSnapshot(str(params["output_folder"]), int(params.get("width", 1024)),
                                                int(params.get("height", 1024)), str(params.get("prefix", "")))
    return {"files": _items(paths)}


def handle_export_rendering_image(params):
    return {"files": [_items(x) for x in _items(export_api.ExportRenderingImage(str(params["file_path"])))]}


def handle_set_fit_maps(params):
    if params.get("stress") is not None:
        utility_api.SetStressMapStatus(bool(params["stress"]))
    if params.get("strain") is not None:
        utility_api.SetStrainMapStatus(bool(params["strain"]))
    utility_api.Refresh3DWindow()
    return {"stress": utility_api.GetStressMapStatus(), "strain": utility_api.GetStrainMapStatus()}


# ---------------------------------------------------------------------------
# EXTENDED - graphics: logos, badges, prints
# ---------------------------------------------------------------------------

def _items(value):
    """A CLO result as a list: lists and tuples as they are, anything else (None, a bool from an old build) empty."""
    return list(value) if isinstance(value, (list, tuple)) else []


def _mapping(params, key):
    """A params entry that must be a JSON object."""
    value = params.get(key)
    if not isinstance(value, dict):
        raise ValueError(key + " must be an object of name: value pairs")
    return value


def _fields(obj):
    """Plain numeric and text fields of a CLO value object, for JSON."""
    out = {}
    for name in dir(obj):
        if not name.startswith("_"):
            value = getattr(obj, name, None)
            if isinstance(value, (bool, int, float, str)):
                out[name] = value
    return out or str(obj)


def handle_import_graphic_style(params):
    ok = import_api.ImportGraphicStyleFromImage(str(params["image_path"]))
    return {"imported": bool(ok), "graphic_style_count": utility_api.GetGraphicStyleCount()}


def handle_import_as_graphic(params):
    import_api.ImportAsGraphic(str(params["image_path"]))
    return {"graphic_items": utility_api.GetTotalGraphicItemQuantity()}


def handle_get_graphic_styles(params):
    count = utility_api.GetGraphicStyleCount()
    styles = [{"index": i, "name": utility_api.GetGraphicStyleName(i),
               "dimensions": _safe(utility_api.GetGraphicStyleDimensions, i)} for i in range(max(count, 0))]
    return {"count": count, "graphic_items": utility_api.GetTotalGraphicItemQuantity(), "styles": styles}


def handle_set_graphic_style(params):
    index, done = int(params["style_index"]), []
    if params.get("name") is not None:
        utility_api.SetGraphicStyleName(index, str(params["name"]))
        done.append("name")
    width, height = params.get("width"), params.get("height")
    if width is not None and height is not None:
        utility_api.SetGraphicStyleDimensions(index, float(width), float(height))
        done.append("dimensions")
    elif width is not None:
        utility_api.SetGraphicStyleWidth(index, float(width), bool(params.get("only_this_side", False)))
        done.append("width")
    elif height is not None:
        utility_api.SetGraphicStyleHeight(index, float(height), bool(params.get("only_this_side", False)))
        done.append("height")
    if params.get("rgba") is not None:
        r, g, b, a = (int(v) for v in params["rgba"])
        if params.get("colorway_index") is not None:
            utility_api.SetGraphicStyleColor(index, int(params["colorway_index"]), r, g, b, a)
        else:
            utility_api.SetGraphicStyleColor(index, r, g, b, a)
        done.append("color")
    return {"style_index": index, "applied": done}


def handle_replace_graphic_style_image(params):
    ok = utility_api.ReplaceGraphicStyleFromImage(int(params["style_index"]), str(params["image_path"]),
                                                  int(params.get("anchor", 0)))
    return {"replaced": bool(ok)}


def handle_set_graphic_style_maps(params):
    index, done = int(params["style_index"]), []
    if params.get("base_color") is not None:
        if params.get("colorway_index") is not None:
            utility_api.SetGraphicBaseColorMapTexture(str(params["base_color"]), index, int(params["colorway_index"]))
        else:
            utility_api.SetGraphicBaseColorMapTexture(str(params["base_color"]), index)
        done.append("base_color")
    setters = {"normal": utility_api.SetGraphicNormalMapTexture,
               "displacement": utility_api.SetGraphicDisplacementMapTexture,
               "opacity": utility_api.SetGraphicOpacityMapTexture,
               "roughness": utility_api.SetGraphicRoughnessMapTexture,
               "metalness": utility_api.SetGraphicMetalnessMapTexture}
    for key, setter in setters.items():
        if params.get(key) is not None:
            setter(index, str(params[key]))
            done.append(key)
    return {"style_index": index, "applied": done}


def handle_get_graphic_style_maps(params):
    index = int(params["style_index"])
    return {"normal": utility_api.GetGraphicNormalMapTexture(index),
            "displacement": utility_api.GetGraphicDisplacementMapTexture(index),
            "opacity": utility_api.GetGraphicOpacityMapTexture(index),
            "roughness": utility_api.GetGraphicRoughnessMapTexture(index),
            "metalness": utility_api.GetGraphicMetalnessMapTexture(index)}


def handle_assign_graphic_style(params):
    style, graphic, option = int(params["style_index"]), int(params["graphic_index"]), int(params.get("assign_option", 0))
    if params.get("colorway_index") is not None:
        utility_api.SetGraphicStyleToGraphic(style, int(params["colorway_index"]), graphic, option)
    else:
        utility_api.SetGraphicStyleToGraphic(style, graphic, option)
    return {"style_index": style, "graphic_index": graphic}


# ---------------------------------------------------------------------------
# EXTENDED - geometry: pieces, lines, annotations
# ---------------------------------------------------------------------------

def handle_copy_pattern_with_offset(params):
    new_index = pattern_api.CopyPatternPieceMove(int(params["pattern_index"]), float(params["dx"]), float(params["dy"]))
    return {"new_index": new_index}


def handle_move_pattern_relative(params):
    pattern_api.SetPatternPieceMove(int(params["pattern_index"]), float(params["dx"]), float(params["dy"]))
    return {"moved": True}


def handle_find_pattern(params):
    if params.get("name") is not None:
        return {"pattern_index": pattern_api.GetPatternIndex(str(params["name"]))}
    x, y = float(params["x"]), float(params["y"])
    if params.get("view", "2d") == "3d":
        return {"pattern_index": pattern_api.GetPatternIndexFrom3DView(x, y)}
    return {"pattern_index": pattern_api.GetPatternIndexFrom2DView(x, y)}


def handle_get_pattern_input_information(params):
    if params.get("pattern_index") is None:
        return {"information": _json_or_text(pattern_api.GetPatternInputInformation())}
    return {"information": _json_or_text(pattern_api.GetPatternInputInformation(int(params["pattern_index"])))}


def handle_set_pattern_category(params):
    index, done = int(params["pattern_index"]), []
    if params.get("category") is not None:
        pattern_api.SetPatternPieceCategory(index, int(params["category"]))
        done.append("category")
    if params.get("classification") is not None:
        pattern_api.SetPatternPieceClassification(index, str(params["classification"]))
        done.append("classification")
    return {"pattern_index": index, "applied": done}


def handle_distribute_internal_lines(params):
    pattern_api.DistribueInternalLinesbetweenSegments(int(params["pattern_index"]),
                                                      [int(i) for i in params["line_indices"]],
                                                      int(params["number"]), bool(params.get("straight", True)),
                                                      bool(params.get("perpendicular", False)),
                                                      bool(params.get("graduate", False)))
    return {"distributed": True}


def handle_add_annotation(params):
    pattern_api.AddPatternAnnotation(int(params["pattern_index"]), float(params["x"]), float(params["y"]),
                                     str(params["text"]))
    return {"added": True}


def handle_edit_annotation(params):
    pattern_api.EditPatternAnnotation(int(params["pattern_index"]), float(params["x"]), float(params["y"]),
                                      int(params["annotation_index"]), str(params["text"]))
    return {"edited": True}


def handle_get_annotations(params):
    items = pattern_api.GetPatternAnnotation(int(params["pattern_index"]))
    return {"annotations": [{"text": a[0], "x": a[1], "y": a[2]} for a in _items(items) if isinstance(a, (list, tuple)) and len(a) >= 3]}


# ---------------------------------------------------------------------------
# EXTENDED - sewing finishes: topstitches, seam taping
# ---------------------------------------------------------------------------

def handle_find_seam_group(params):
    return {"seam_group_index": pattern_api.GetSeamlinePairGroupIndexFromName(str(params["name"]))}


def handle_add_seam_topstitch(params):
    ok = pattern_api.AddSeamlineTopstitch(int(params["seam_group_index"]), float(params.get("start_ratio", 0.0)),
                                          float(params.get("end_ratio", 1.0)), int(params.get("style_index", 0)))
    return {"added": bool(ok)}


def handle_import_topstitch_style(params):
    return {"imported": bool(pattern_api.ImportTopStitchStyle(str(params["file_path"])))}


def handle_get_topstitches(params):
    out = {"styles": _safe(pattern_api.GetTopstitchStyleList)}
    if params.get("pattern_index") is not None:
        index = int(params["pattern_index"])
        out.update(count=_safe(pattern_api.GetPatternAssignedTopstitchCount, index),
                   topstitches=_safe(pattern_api.GetPatternAssignedTopstitch, index),
                   assigned_styles=_safe(pattern_api.GetPatternAssignedTopstitchStyle, index))
    return out


def handle_set_topstitch(params):
    index, stitch, done = int(params["pattern_index"]), int(params["stitch_index"]), []
    setters = [("style_index", pattern_api.SetPatternAssignedTopstitchStyle, int),
               ("curved", pattern_api.SetPatternAssignedTopstitchCurved, bool),
               ("curved_length", pattern_api.SetPatternAssignedTopstitchCurvedLength, int),
               ("right_angled", pattern_api.SetPatternAssignedTopstitchCurvedRightAngled, bool),
               ("extend_start", pattern_api.SetPatternAssignedTopstitchExtendStart, bool),
               ("extend_end", pattern_api.SetPatternAssignedTopstitchExtendEnd, bool),
               ("z_offset", pattern_api.SetPatternAssignedTopstitchZOffset, float)]
    for key, setter, cast in setters:
        if params.get(key) is not None:
            setter(index, stitch, cast(params[key]))
            done.append(key)
    return {"pattern_index": index, "stitch_index": stitch, "applied": done}


def handle_set_topstitch_style_color(params):
    style, line = int(params["style_index"]), int(params.get("line_index", -1))
    colorway = params.get("colorway_index")
    if params.get("rgba") is not None:
        r, g, b, a = (int(v) for v in params["rgba"])
        if colorway is not None:
            utility_api.SetTopStitchColor(style, int(colorway), line, r, g, b, a)
        else:
            utility_api.SetTopStitchColor(style, line, r, g, b, a)
    if params.get("opacity") is not None:
        if colorway is not None:
            utility_api.SetTopStitchOpacity(style, int(colorway), line, int(params["opacity"]))
        else:
            utility_api.SetTopStitchOpacity(style, line, int(params["opacity"]))
    return {"style_index": style}


def handle_get_all_stitches(params):
    return {"stitches": _json_or_text(pattern_api.GetAllStitchProperty())}


def handle_set_seam_taping(params):
    index, line = int(params["pattern_index"]), int(params.get("line_index", -1))
    pattern_api.SetPatternPieceSeamtaping(index, line, bool(params.get("enabled", True)))
    if params.get("width") is not None:
        pattern_api.SetPatternPieceSametapingWidth(index, line, float(params["width"]))
    return {"pattern_index": index, "line_index": line}


# ---------------------------------------------------------------------------
# EXTENDED - simulation: layers, states, stiffness, shrinkage, settings
# ---------------------------------------------------------------------------

def handle_set_pattern_state(params):
    index, done = int(params["pattern_index"]), []
    setters = [("layer", pattern_api.SetPatternLayer, int),
               ("freeze", pattern_api.SetPatternFreeze, bool),
               ("lock", pattern_api.SetPatternLock, bool),
               ("hide_3d", pattern_api.SetPatternHide3D, bool),
               ("solidify", pattern_api.SetPatternPieceSolidify, bool),
               ("solidify_strength", pattern_api.SetPatternPieceSolidifyStrengthen, float),
               ("strengthen", pattern_api.SetPatternStrengthen, bool),
               ("grain_link_all_colorways", pattern_api.SetPatternPieceGrainLinkAllColorways, bool)]
    for key, setter, cast in setters:
        if params.get(key) is not None:
            setter(index, cast(params[key]))
            done.append(key)
    return {"pattern_index": index, "applied": done}


def handle_set_particle_distance_all(params):
    pattern_api.SetParticleDistanceOfPatterns(float(params["distance"]))
    return {"particle_distance": float(params["distance"])}


def handle_set_shrinkage(params):
    index = int(params["pattern_index"])
    if params.get("width_percent") is not None:
        pattern_api.SetWidthShrinkagePercentage(index, float(params["width_percent"]))
    if params.get("height_percent") is not None:
        pattern_api.SetHeightShrinkagePercentage(index, float(params["height_percent"]))
    return {"shrinkage": _safe(pattern_api.GetShrinkagePercentage, index)}


def handle_get_simulation_quality(params):
    q = _items(utility_api.GetSimulationQuality())
    if len(q) != 2:
        return {"raw": str(utility_api.GetSimulationQuality())}
    return {"quality": q[0], "simulation_mode": q[1]}


def handle_set_simulation_settings(params):
    setters = [("time_step", utility_api.SetSimulationTimeStep, float),
               ("substeps", utility_api.SetSimulationNumberOfSimulation, int),
               ("cg_iterations", utility_api.SetSimulationCGIterationCount, int),
               ("cg_residual", utility_api.SetSimulationCGResidual, float),
               ("cg_finish_condition", utility_api.SetSimulationCGFinishCondition, int),
               ("self_collision_iterations", utility_api.SetSimulationSelfCollisionIterationCount, int),
               ("self_collision_stiffness", utility_api.SetSimulationSelfCollisionAvoidanceStiffness, float),
               ("layer_based_collision", utility_api.SetSimulationLayerBasedCollisionDetection, bool),
               ("nonlinear", utility_api.SetSimulationNonlinearSimulation, bool),
               ("air_damping", utility_api.SetSimulationAirDamping, float),
               ("gravity", utility_api.SetSimulationGravity, float),
               ("ground_collision", utility_api.SetSimulationGroundCollision, bool),
               ("ground_height", utility_api.SetSimulationGroundHeight, float),
               ("cpu_count", utility_api.SetSimulationNumberOfCPUInUse, int)]
    done = []
    for key, setter, cast in setters:
        if params.get(key) is not None:
            setter(cast(params[key]))
            done.append(key)
    return {"applied": done}


def handle_set_mesh_type(params):
    pattern_api.SetMeshType(int(params["pattern_index"]), str(params["mesh_type"]))
    return {"mesh_counts": _safe(pattern_api.GetMeshCountByType, int(params["pattern_index"]))}


def handle_get_mesh_counts(params):
    return {"mesh_counts": _safe(pattern_api.GetMeshCountByType, int(params["pattern_index"]))}


# ---------------------------------------------------------------------------
# EXTENDED - fabrics
# ---------------------------------------------------------------------------

def handle_change_fabric_with_json(params):
    if params.get("colorway_index") is not None:
        ok = fabric_api.ChangeFabricWithJson(int(params["colorway_index"]), int(params["fabric_index"]),
                                             str(params["jfab_path"]))
    else:
        ok = fabric_api.ChangeFabricWithJson(int(params["fabric_index"]), str(params["jfab_path"]))
    return {"changed": bool(ok)}


def handle_export_fabric(params):
    path = str(params["file_path"])
    if params.get("colorway_index") is not None:
        out = fabric_api.ExportFabric(path, int(params["colorway_index"]), int(params["fabric_index"]))
    elif params.get("fabric_index") is not None:
        out = fabric_api.ExportFabric(path, int(params["fabric_index"]))
    else:
        out = fabric_api.ExportFabric(path)
    return {"file_path": out}


def handle_get_fabric_details(params):
    index = int(params["fabric_index"])
    colorway = params.get("colorway_index")
    if colorway is not None:
        cw = int(colorway)
        return {"name": _safe(fabric_api.GetFabricName, cw, index),
                "information": _safe(fabric_api.GetFabricInformation, cw, index),
                "width_mm": _safe(fabric_api.GetFabricWidth, cw, index),
                "length_mm": _safe(fabric_api.GetFabricLength, cw, index)}
    return {"name": _safe(fabric_api.GetFabricName, index),
            "info": _json_or_text(_safe(fabric_api.GetFabricInfo, index)),
            "information": _safe(fabric_api.GetFabricInformation, index),
            "width_mm": _safe(fabric_api.GetFabricWidth, index),
            "length_mm": _safe(fabric_api.GetFabricLength, index)}


def handle_set_fabric(params):
    index, done = int(params["fabric_index"]), []
    colorway = params.get("colorway_index")
    if params.get("name") is not None:
        if colorway is not None:
            fabric_api.SetFabricName(int(colorway), index, str(params["name"]))
        else:
            fabric_api.SetFabricName(index, str(params["name"]))
        done.append("name")
    if params.get("width_mm") is not None:
        if colorway is not None:
            fabric_api.SetFabricWidth(int(colorway), index, float(params["width_mm"]))
        else:
            fabric_api.SetFabricWidth(index, float(params["width_mm"]))
        done.append("width")
    if params.get("information") is not None:
        info = {str(k): str(v) for k, v in params["information"].items()}
        if colorway is not None:
            fabric_api.SetFabricInformation(int(colorway), index, info)
        else:
            fabric_api.SetFabricInformation(index, info)
        done.append("information")
    return {"fabric_index": index, "applied": done}


def handle_find_fabric(params):
    if params.get("colorway_index") is not None:
        return {"fabric_index": fabric_api.GetFabricIndex(int(params["colorway_index"]), str(params["name"]))}
    return {"fabric_index": fabric_api.GetFabricIndex(str(params["name"]))}


def handle_set_pattern_fabric(params):
    pattern_api.SetPatternPieceFabricIndex(int(params["pattern_index"]), int(params["fabric_index"]))
    return {"pattern_index": int(params["pattern_index"]), "fabric_index": int(params["fabric_index"])}


def handle_get_primary_fabric(params):
    return {"fabric_index": fabric_api.GetPrimaryFabric()}


# ---------------------------------------------------------------------------
# EXTENDED - trims and buttons
# ---------------------------------------------------------------------------

def handle_import_trim(params):
    import_api.ImportTrim(str(params["file_path"]), bool(params.get("translate_to_origin", True)))
    return {"imported": True}


def handle_export_trim(params):
    return {"file_path": export_api.ExportTrim(str(params["file_path"]), int(params["trim_style_index"]))}


def handle_set_trim_style(params):
    style, done = int(params["style_index"]), []
    if params.get("rgba") is not None:
        r, g, b, a = (int(v) for v in params["rgba"])
        mesh = int(params.get("mesh_index", -1))
        if params.get("colorway_index") is not None:
            utility_api.SetTrimStyleColor(style, int(params["colorway_index"]), mesh, r, g, b, a)
        else:
            utility_api.SetTrimStyleColor(style, mesh, r, g, b, a)
        done.append("color")
    if params.get("weight") is not None:
        utility_api.SetTrimWeight(style, float(params["weight"]))
        done.append("weight")
    return {"style_index": style, "applied": done}


def handle_set_button_colors(params):
    done = []
    colorway = params.get("colorway_index")
    if params.get("head_style_index") is not None and params.get("head_rgba") is not None:
        r, g, b, a = (int(v) for v in params["head_rgba"])
        style, mesh = int(params["head_style_index"]), int(params.get("mesh_index", -1))
        if colorway is not None:
            utility_api.SetButtonHeadStyleColor(style, int(colorway), mesh, r, g, b, a)
        else:
            utility_api.SetButtonHeadStyleColor(style, mesh, r, g, b, a)
        done.append("head")
    if params.get("hole_style_index") is not None and params.get("hole_rgba") is not None:
        r, g, b, a = (int(v) for v in params["hole_rgba"])
        style = int(params["hole_style_index"])
        if colorway is not None:
            utility_api.SetButtonHoleStyleColor(style, int(colorway), r, g, b, a)
        else:
            utility_api.SetButtonHoleStyleColor(style, r, g, b, a)
        done.append("hole")
    return {"applied": done}


# ---------------------------------------------------------------------------
# EXTENDED - grading, garment information, bill of materials, poses
# ---------------------------------------------------------------------------

def handle_get_grading_sizes(params):
    groups = _safe(pattern_api.GetGradingSizeGroupInformation)
    out = {"total": _safe(pattern_api.GetGradingSizeTotalCount),
           "names": _safe(pattern_api.GetGradingSizeNameList),
           "current_index": _safe(pattern_api.GetCurrentGradingSizeIndex),
           "groups": groups,
           "valid_groups": _safe(pattern_api.GetValidGradingSizeGroupInformation)}
    if isinstance(groups, list):
        out["sizes_by_group"] = {str(g): _safe(pattern_api.GetGradingSizeListFromRuleTable, i)
                                 for i, g in enumerate(groups)}
    return out


def handle_change_grading_size(params):
    group, size = params["group"], params["size"]
    if isinstance(group, int) and isinstance(size, int):
        ok = pattern_api.ChangeGradingSizeInformation(int(group), int(size))
    else:
        ok = pattern_api.ChangeGradingSizeInformation(str(group), str(size))
    return {"changed": bool(ok)}


def handle_get_grading_paired_avatar(params):
    return {"avatar": pattern_api.GetGradingPairedAvatar(int(params["grading_index"]))}


def handle_export_garment_information(params):
    if params.get("file_path"):
        return {"file_path": export_api.ExportGarmentInformation(str(params["file_path"]))}
    return {"file_path": export_api.ExportGarmentInformation()}


def handle_import_garment_information(params):
    return {"imported": bool(import_api.ImportGarmentInformation(str(params["file_path"])))}


def handle_export_bom(params):
    return {"exported": bool(export_api.ExportBOM(str(params["file_path"]))), "file_path": params["file_path"]}


def handle_import_pose(params):
    return {"imported": bool(import_api.ImportPose(str(params["file_path"])))}


def handle_export_pose(params):
    if params.get("file_path"):
        return {"file_path": export_api.ExportPose(str(params["file_path"]))}
    return {"file_path": export_api.ExportPose()}


# ---------------------------------------------------------------------------
# EXTENDED - technical drawing, colorway images, nesting, colorway colors
# ---------------------------------------------------------------------------

def handle_set_schematic(params):
    setters = [("render", utility_api.SetSchematicRender, bool),
               ("seam_lines", utility_api.SetShowSchematicSeamLine, bool),
               ("internal_lines", utility_api.SetShowSchematicInternalLine, bool),
               ("topstitch_lines", utility_api.SetShowSchematicTopstitchLine, bool),
               ("silhouette", utility_api.SetShowSchematicSilhouetteLine, bool),
               ("seam_line_width", utility_api.SetSchematicSeamLineWidth, int),
               ("internal_line_width", utility_api.SetSchematicInternalLineWidth, int),
               ("silhouette_width", utility_api.SetSchematicSilhouetteLineWidth, int),
               ("topstitch_scale_percent", utility_api.SetSchematicTopstitchLineScalePercent, int)]
    done = []
    for key, setter, cast in setters:
        if params.get(key) is not None:
            setter(cast(params[key]))
            done.append(key)
    utility_api.Refresh3DWindow()
    return {"applied": done, "render": utility_api.GetSchematicRender()}


def handle_export_view_images_all_colorways(params):
    view = int(params["view_index"])
    return {"result": export_api.ExportStdViewImageForAllColorways(
        view, _image_path(params["output_path"], "view_%d.png" % view),
        int(params.get("width", 1024)), int(params.get("height", 1024)))}


def handle_start_nesting(params):
    if params.get("buffer_spacing") is not None:
        utility_api.SetNestingBufferSpacing(float(params["buffer_spacing"]))
    if params.get("target_colorways") is not None:
        utility_api.SetNestingTargetColorway([int(i) for i in params["target_colorways"]])
    utility_api.StartNesting()
    return {"started": True}


def handle_stop_nesting(params):
    utility_api.StopNesting()
    return {"stopped": True, "time_ms": _safe(utility_api.GetNestingTime)}


def handle_get_nesting_status(params):
    return {"time_ms": _safe(utility_api.GetNestingTime), "buffer_spacing": _safe(utility_api.GetNestingBufferSpacing),
            "target_colorway": _safe(utility_api.GetNestingTargetColorway)}


def handle_set_nesting_pattern(params):
    index, done = int(params["pattern_index"]), []
    if params.get("x") is not None and params.get("y") is not None:
        pattern_api.SetNestingFixedPatternPiecePos(index, int(params["x"]), int(params["y"]))
        done.append("fixed_position")
    if params.get("grain_mode") is not None:
        pattern_api.SetNestingPatternPieceGrainDirection(index, int(params["grain_mode"]))
        done.append("grain_mode")
    return {"pattern_index": index, "applied": done}


def handle_get_colorway_colors(params):
    # GetColorListForColorWay returns a type Python cannot convert: read the color items one by one.
    index = int(params["colorway_index"])
    items = []
    for i in range(int(params.get("max_items", 64))):
        name = _safe(utility_api.GetColorwayColorItemName, index, i)
        if not isinstance(name, str) or name.startswith("error:") or name == "":
            break
        items.append({"index": i, "name": name, "rgb": _safe(utility_api.GetColorwayColorItemRGB, index, i)})
    return {"colorway_index": index, "name": _safe(utility_api.GetColorwayName, index), "items": items}


def handle_update_colorways(params):
    utility_api.UpdateColorways(bool(params.get("update_snapshot", True)))
    return {"updated": True}


# ---------------------------------------------------------------------------
# EXTENDED - undocumented functions (signatures read live with api_doc)
# ---------------------------------------------------------------------------

def handle_instance_pattern(params):
    if params.get("name") is not None:
        pattern_api.InstancePatternPieceWithPatternName(str(params["name"]))
    else:
        pattern_api.InstancePatternPiece(int(params["pattern_index"]))
    return {"pattern_count": pattern_api.GetPatternCount()}


def handle_symmetry_pattern(params):
    flag = bool(params.get("flag", True))
    if params.get("name") is not None:
        pattern_api.SymmetryPatternPieceWithPatternName(str(params["name"]), flag)
    else:
        pattern_api.SymmetryPatternPiece(int(params["pattern_index"]), flag)
    return {"pattern_count": pattern_api.GetPatternCount()}


def handle_unfold_pattern_by_name(params):
    ok = pattern_api.UnfoldPatternPieceWithPatternName(str(params["name"]), int(params["line_index"]),
                                                       bool(params.get("half_symmetry", False)))
    return {"unfolded": bool(ok)}


def handle_get_selected_patterns(params):
    count = pattern_api.GetPatternCount()
    return {"names": _safe(pattern_api.GetSelectedPattern),
            "indices": [i for i in range(count) if pattern_api.GetSelectedPatternViaIndex(i)]}


def handle_select_pattern(params):
    selected = bool(params.get("selected", True))
    if params.get("name") is not None:
        pattern_api.SelectPatternViaName(str(params["name"]), selected)
    else:
        pattern_api.SelectPatternViaIndex(int(params["pattern_index"]), selected)
    return {"selected": selected}


def handle_get_pin_count(params):
    return {"pins": pattern_api.GetPinListSize()}


def handle_remove_pins(params):
    ok = pattern_api.RemoveAllPins() if params.get("all", True) else pattern_api.RemovePin()
    return {"removed": bool(ok), "remaining": pattern_api.GetPinListSize()}


def handle_set_collision_thickness(params):
    index = int(params["pattern_index"])
    pattern_api.SetAddlThicknessCollision(index, float(params["thickness"]))
    return {"thickness": pattern_api.GetAddlThicknessCollisionValue(index)}


def handle_export_snapshot_2d(params):
    path = _image_path(params["output_path"], "patterns_2d.png")
    files = list(export_api.ExportSnapshot2D(path, int(params.get("option", 0))))
    if not files and os.path.exists(path):  # CLO writes the file but may return an empty list
        files = [path]
    return {"files": files}


def handle_export_zpac(params):
    return {"file_path": export_api.ExportZPacW(str(params["file_path"]))}


def _require_avatar():
    # Without an avatar, CLO opens a blocking "Please Load Avatar" dialog: fail early instead.
    if not export_api.GetAvatarCount():
        raise ValueError("No avatar loaded in CLO3D. Load one first (import_avatar).")


def handle_align_avatar_and_garment(params):
    _require_avatar()
    utility_api.AlignAvatarsAndGarmentToCenter()
    return {"aligned": True}


def handle_get_avatar_properties(params):
    _require_avatar()
    return {"properties": dict(utility_api.GetAvatarProperties(int(params["avatar_index"])))}


def handle_set_avatar_properties(params):
    _require_avatar()
    index = int(params["avatar_index"])
    utility_api.SetAvatarProperties(index, {str(k): str(v) for k, v in _mapping(params, "properties").items()})
    return {"properties": dict(utility_api.GetAvatarProperties(index))}


def handle_delete_avatars(params):
    return {"deleted": bool(utility_api.DeleteAvatar([int(i) for i in params["avatar_indices"]]))}


def handle_set_avatar_activation(params):
    return {"done": bool(utility_api.SetAvatarActivation(str(params["name"]), bool(params.get("active", True))))}


def handle_set_avatar_softness(params):
    _require_avatar()
    index = int(params["avatar_index"])
    if params.get("stiffness") is not None:
        utility_api.SetAvatarSoftBodyStiffness(index, float(params["stiffness"]))
    return {"stiffness": utility_api.GetAvatarSoftBodyStiffness(index)}


def handle_get_topstitch_style_details(params):
    # Second index of the getters: stitch row, starting at 1. GetTopStitchCount(False) lists every style.
    sub = int(params.get("line_index", 1))
    count = utility_api.GetTopStitchCount(bool(params.get("flag", False)))
    styles = [{"index": i, "name": _safe(utility_api.GetTopStitchName, i),
               "number_of_lines": _safe(utility_api.GetTopStitchNumberOfLines, i, sub),
               "distance": _safe(utility_api.GetTopStitchDistanceValue, i, sub),
               "width": _safe(utility_api.GetTopStitchWidthValue, i, sub),
               "offset_index": _safe(utility_api.GetTopStitchOffsetIndex, i),
               "offset_value": _safe(utility_api.GetTopStitchOffsetValue, i)} for i in range(max(count, 0))]
    return {"count": count, "styles": styles}


def handle_set_topstitch_style(params):
    style, sub, done = int(params["style_index"]), int(params.get("line_index", 1)), []
    if params.get("name") is not None:
        utility_api.SetTopStitchName(style, str(params["name"]))
        done.append("name")
    if params.get("number_of_lines") is not None:
        utility_api.SetTopStitchNumberOfLines(style, sub, int(params["number_of_lines"]))
        done.append("number_of_lines")
    if params.get("distance") is not None:
        utility_api.SetTopStitchDistanceValue(style, sub, float(params["distance"]))
        done.append("distance")
    if params.get("width") is not None:
        utility_api.SetTopStitchWidthValue(style, sub, float(params["width"]))
        done.append("width")
    if params.get("offset_index") is not None:
        utility_api.SetTopStitchOffsetIndex(style, int(params["offset_index"]))
        done.append("offset_index")
    return {"style_index": style, "applied": done}


def handle_get_trim_styles(params):
    count = utility_api.GetTrimStyleCount(bool(params.get("flag", True)))
    return {"count": count, "styles": [{"index": i, "name": _safe(utility_api.GetTrimStyleName, i)}
                                       for i in range(max(count, 0))]}


def handle_find_trim_style(params):
    return {"style_index": utility_api.GetTrimStyleIndex(str(params["name"]))}


def handle_get_trim_material(params):
    return {"properties": dict(utility_api.GetTrimMaterialProperties(int(params["style_index"]),
                                                                     int(params.get("mesh_index", 0))))}


def handle_set_trim_material(params):
    style, mesh = int(params["style_index"]), int(params.get("mesh_index", 0))
    utility_api.SetTrimMaterialProperties(style, mesh, {str(k): str(v) for k, v in _mapping(params, "properties").items()})
    return {"properties": dict(utility_api.GetTrimMaterialProperties(style, mesh))}


def handle_set_zipper_style(params):
    style, done = int(params["style_index"]), []
    setters = [("name", utility_api.SetZipperStyleName, str),
               ("teeth_type", utility_api.SetZipperStyleTeethType, int),
               ("teeth_width", utility_api.SetZipperStyleTeethWidth, float),
               ("tape_thickness", utility_api.SetZipperStyleTapeThickness, float),
               ("weight", utility_api.SetZipperStyleWeight, float),
               ("slider", utility_api.SetZipperSliderStyle, str),
               ("puller", utility_api.SetZipperPullerStyle, str),
               ("top_stopper", utility_api.SetZipperTopStopperStyle, str),
               ("bottom_stopper", utility_api.SetZipperBottomStopperStyle, str)]
    for key, setter, cast in setters:
        if params.get(key) is not None:
            setter(style, cast(params[key]))
            done.append(key)
    return {"style_index": style, "name": _safe(utility_api.GetZipperStyleName, style), "applied": done}


def handle_add_graphic_to_pattern(params):
    # CLO 2025.2: AddGraphicStyleToPattern(pattern, style, x, y, width, height, face, z_offset, angle).
    # Verified live: pattern first. (x, y) is a fixed point of the logo (size does not move it), measured from the
    # pattern bounding box; the mapping is approximate. Fails on pieces created by script in tests.
    pattern, style = int(params["pattern_index"]), int(params["style_index"])
    ok = utility_api.AddGraphicStyleToPattern(pattern, style, float(params["x"]), float(params["y"]),
                                              float(params["width"]), float(params["height"]),
                                              int(params.get("face", 0)), float(params.get("z_offset", 0.0)),
                                              float(params.get("angle", 0.0)))
    placed = dict(utility_api.GetGraphicStylePosition(style, True)).get(pattern, [])
    return {"added": bool(ok), "positions_on_pattern": [list(p) for p in placed]}


def handle_get_graphic_positions(params):
    raw = utility_api.GetGraphicStylePosition(int(params["style_index"]), bool(params.get("flag", True)))
    raw = raw if isinstance(raw, dict) else {}
    return {"positions": {str(k): [_items(p) for p in _items(v)] for k, v in raw.items()}}


def handle_find_api_types(params):
    """Read-only: names containing a text in every loaded *_api module and in builtins (option classes)."""
    text = str(params["text"]).lower()
    found = {}
    candidates = {name: mod for name, mod in sys.modules.items() if mod is not None}
    for name, mod in candidates.items():
        try:
            names = [n for n in dir(mod) if text in n.lower()]
        except Exception:
            continue
        if names:
            found[name] = sorted(names)[:50]
    return {"found": found}


# ---------------------------------------------------------------------------
# EXTENDED - fabric materials, sewn patches, CLO library
# ---------------------------------------------------------------------------

def _fabric_call(function, params, *args):
    """Call a fabric setter with the colorway overload when colorway_index is given."""
    if params.get("colorway_index") is not None:
        return function(int(params["colorway_index"]), *args)
    return function(*args)


def handle_set_current_fabric(params):
    index = int(params["fabric_index"])
    if params.get("colorway_index") is not None:
        ok = fabric_api.SetCurrentFabricIndex(int(params["colorway_index"]), index)
    else:
        ok = fabric_api.SetCurrentFabricIndex(index)
    return {"selected": bool(ok), "current": fabric_api.GetCurrentFabricIndex()}


def handle_set_fabric_maps(params):
    index, done = int(params["fabric_index"]), []
    setters = {"base_color": fabric_api.SetBaseTextureMapImageGivenFilePath,
               "normal": fabric_api.SetNormalMapImageGivenFilePath,
               "displacement": fabric_api.SetDisplacementMapImageGivenFilePath,
               "opacity": fabric_api.SetOpacityMapImageGivenFilePath,
               "roughness": fabric_api.SetRoughnessMapImageGivenFilePath,
               "metalness": fabric_api.SetMetalnessMapImageGivenFilePath}
    for key, setter in setters.items():
        if params.get(key) is not None:
            if params.get("colorway_index") is not None:
                setter(str(params[key]), int(params["colorway_index"]), index)
            else:
                setter(str(params[key]), index)
            done.append(key)
    return {"fabric_index": index, "applied": done}


def handle_get_fabric_maps(params):
    index = int(params["fabric_index"])
    if params.get("colorway_index") is not None:
        cw = int(params["colorway_index"])
        return {"base_color": _safe(fabric_api.GetBaseTextureMapImageFilePath, cw, index),
                "normal": _safe(fabric_api.GetNormalMapImageFilePath, cw, index),
                "opacity": _safe(fabric_api.GetOpacityMapImageFilePath, cw, index)}
    return {"base_color": _safe(fabric_api.GetBaseTextureMapImageFilePath, index),
            "normal": _safe(fabric_api.GetNormalMapImageFilePath, index),
            "opacity": _safe(fabric_api.GetOpacityMapImageFilePath, index)}


def handle_set_fabric_material(params):
    index, face, done = int(params["fabric_index"]), int(params.get("face", 0)), []
    setters = [("material_type", fabric_api.SetMaterialType, int),
               ("metalness", fabric_api.SetMetalness, float),
               ("roughness_type", fabric_api.SetRoughnessType, int),
               ("roughness_intensity", fabric_api.SetRoughnessValueIntensity, int),
               ("roughness_map_intensity", fabric_api.SetRoughnessValueMapIntensity, int),
               ("roughness_map_invert", fabric_api.SetRoughnessValueMapInvert, bool),
               ("reflection_intensity", fabric_api.SetReflectionIntensity, float),
               ("reflection_roughness", fabric_api.SetReflectionRoughness, float),
               ("normal_intensity", fabric_api.SetNormalMapIntensity, float),
               ("opacity_intensity", fabric_api.SetOpacityIntensity, float),
               ("same_color_as_front", fabric_api.SetUseSameColorAsFront, bool),
               ("same_material_as_front", fabric_api.SetUseSameMaterialAsFront, bool)]
    for key, setter, cast in setters:
        if params.get(key) is not None:
            _fabric_call(setter, params, index, face, cast(params[key]))
            done.append(key)
    return {"fabric_index": index, "face": face, "applied": done,
            "metalness": _safe(fabric_api.GetMetalness, index, face),
            "material_type": _safe(fabric_api.GetMaterialType, index, face)}


def handle_set_fabric_displacement(params):
    index, out = int(params["fabric_index"]), {}
    if params.get("image_path") is not None:
        out["map"] = bool(_fabric_call(fabric_api.SetPBRMaterialDisplacementMap, params, index, str(params["image_path"])))
    if params.get("amount") is not None:
        out["values"] = bool(_fabric_call(fabric_api.SetPBRMaterialDisplacementMapValue, params, index,
                                          float(params["amount"]), float(params.get("shift", 0.0)),
                                          float(params.get("clipping", 0.0)),
                                          float(params.get("particle_distance", 5.0)), bool(params.get("keep", True))))
    return out


def handle_set_texture_mapping(params):
    _fabric_call(fabric_api.SetTextureMapping, params, int(params["fabric_index"]), int(params["mapping_type"]))
    return {"mapping_type": int(params["mapping_type"])}


def handle_import_substance(params):
    index, path = int(params["fabric_index"]), str(params["file_path"])
    if params.get("face_type"):
        ok = _fabric_call(fabric_api.ImportSubstanceFileAsFaceType, params, index, path, str(params["face_type"]))
    else:
        ok = _fabric_call(fabric_api.ImportSubstanceFile, params, index, path)
    return {"imported": bool(ok)}


def handle_set_substance(params):
    index, face, done = int(params["fabric_index"]), int(params.get("face", 0)), []
    if params.get("preset_index") is not None:
        _fabric_call(fabric_api.SetSubstancePreset, params, index, face, int(params["preset_index"]))
        done.append("preset")
    if params.get("resolution_index") is not None:
        _fabric_call(fabric_api.SetSubstanceResolution, params, index, face, int(params["resolution_index"]))
        done.append("resolution")
    return {"applied": done}


def handle_set_fabric_custom_image(params):
    _fabric_call(fabric_api.SetCustomImage, params, int(params["fabric_index"]), str(params["image_path"]))
    return {"set": True}


def handle_duplicate_fabric(params):
    # No copy function in the API: save the fabric to a .zfab, then add that file as a new fabric.
    source = int(params["fabric_index"])
    folder = str(params.get("folder") or utility_api.GetCLOTemporaryFolderPath())
    name = str(params.get("name") or "fabric_copy")
    path = os.path.join(folder, name + ".zfab")
    saved = fabric_api.ExportFabric(path, source)
    new_index = fabric_api.AddFabric(saved or path)
    fabric_api.SetFabricName(new_index, name)
    return {"fabric_index": new_index, "file_path": saved or path, "fabric_count": fabric_api.GetFabricCount(True)}


def handle_get_clo_folders(params):
    return {"assets": _safe(utility_api.GetCLOAssetFolderPath, True),
            "temporary": _safe(utility_api.GetCLOTemporaryFolderPath),
            "executable": _safe(utility_api.GetCLOExecutableFolderPath, True)}


def handle_find_assets(params):
    """Read-only: files of CLO's asset library with the given extensions."""
    root = str(params.get("root") or utility_api.GetCLOAssetFolderPath(True))
    extensions = tuple(e.lower() if e.startswith(".") else "." + e.lower() for e in params["extensions"])
    limit, found = int(params.get("limit", 100)), []
    for folder, _, files in os.walk(root):
        for f in files:
            if f.lower().endswith(extensions):
                found.append(os.path.join(folder, f))
                if len(found) >= limit:
                    return {"root": root, "files": found, "truncated": True}
    return {"root": root, "files": found, "truncated": False}


# -- Read-only documentation of API functions --

def handle_api_doc(params):
    module = sys.modules.get(str(params["module"]))
    if module is None or not str(params["module"]).endswith("_api"):
        raise ValueError("Unknown CLO API module: " + str(params["module"]))
    return {name: (getattr(getattr(module, name, None), "__doc__", None) or "not found")
            for name in params["functions"]}


def handle_list_api(params):
    """Public names of every CLO API module loaded in this Python (read-only introspection)."""
    for name in ("export_api", "fabric_api", "import_api", "pattern_api", "utility_api", "rest_api"):
        try:
            __import__(name)
        except ImportError:
            pass
    return {name: sorted(n for n in dir(mod) if not n.startswith("_"))
            for name, mod in sorted(sys.modules.items()) if name.endswith("_api") and mod is not None}



# ---------------------------------------------------------------------------
# EXTENDED: whole garment in one call, scene summary
# ---------------------------------------------------------------------------
# Garment JSON (written by pattern-making-skill, clo3d_<size>.json): {"unit": "mm", "pieces": [...],
# "seams": [...], "holds": [...]}. Piece: {"name", "points": [[x, y, type]...], "edges": [{"from": node}...],
# "fabric": role, "particle_distance": mm, "layer": int, "internals": [[[x, y]...]...] (internal lines),
# "arrange": {"name" | "index", "orientation", "x", "y", "offset"}}. Seam: [piece A, node a1, node a2, piece B, node b1, node b2, {...}], a1 sewn to b1.
# Hold: {"top", "under", "edge": [a, b], "at": {a: [x, y], b: [x, y]}, "layer"}: the top piece's edge is sewn
# onto an internal line of the under piece (overlapping fronts).

FAR_MM = 2500.0  # a piece created this far from the 2D origin lands metres from the avatar in 3D and falls


def _norm_key(k):
    return str(k).replace(" ", "").replace("_", "").lower()


def _arrangement_names():
    """Arrangement point names of the loaded avatar, in index order."""
    names = []
    for item in _items(pattern_api.GetArrangementList()):
        name = None
        if isinstance(item, dict):
            for k, v in item.items():
                if _norm_key(k) in ("arrangementname", "name"):
                    name = str(v)
        elif isinstance(item, str):
            name = item
        names.append(name if name is not None else str(item))
    return names


def _fabric_names():
    count = fabric_api.GetFabricCount(True)
    return [_safe(fabric_api.GetFabricName, i) for i in range(count)]


def _resolve_fabric(value, names):
    """A fabric given as an index or as a name (exact, then case-insensitive substring)."""
    if isinstance(value, (int, float)):
        return int(value)
    v = str(value)
    if v in names:
        return names.index(v)
    low = [str(n).lower() for n in names]
    for i, n in enumerate(low):
        if v.lower() in n:
            return i
    raise ValueError("fabric not found: " + v + " (fabrics: " + ", ".join(str(n) for n in names) + ")")


def _resolve_arrangement(spec, names):
    """Arrangement point index from {"index": i} or {"name": s}; the name is matched exactly, then
    case-insensitively, then as a substring. Returns (index, name) or (None, None)."""
    if spec.get("index") is not None:
        i = int(spec["index"])
        return i, (names[i] if 0 <= i < len(names) else None)
    want = spec.get("name")
    if not want:
        return None, None
    if want in names:
        return names.index(want), want
    low = [n.lower() for n in names]
    if want.lower() in low:
        i = low.index(want.lower())
        return i, names[i]
    for i, n in enumerate(low):
        if want.lower() in n:
            return i, names[i]
    return None, None


def _garment_edge(piece, n1, n2):
    """Line index and direction of the edge n1-n2 of a garment piece (edge i runs from edges[i].from to
    edges[i+1].from). Names split by a hole slit may carry a trailing '~'."""
    e = piece["edges"]
    for a in (n1, n1 + "~"):
        for b in (n2, n2 + "~"):
            for i, x in enumerate(e):
                nxt = e[(i + 1) % len(e)]["from"]
                if (x["from"], nxt) == (a, b):
                    return i, True
                if (x["from"], nxt) == (b, a):
                    return i, False
    raise ValueError(piece["name"] + ": no edge " + n1 + "-" + n2)


def handle_build_garment(params):
    """Create every piece of a garment JSON, name it, set its collision thickness, mesh, fabric, layer and
    arrangement point, then sew every seam and hold. One request instead of hundreds."""
    if params.get("file_path"):
        with open(str(params["file_path"]), "r") as f:
            data = json.load(f)
    else:
        data = params["garment"]
    pieces = data.get("pieces", [])
    if not pieces:
        raise ValueError("garment has no pieces")
    if not params.get("allow_far", False):
        for p in pieces:
            for q in p["points"]:
                if abs(float(q[0])) > FAR_MM or abs(float(q[1])) > FAR_MM:
                    raise ValueError(p["name"] + " has a point " + str(int(max(abs(q[0]), abs(q[1])))) +
                                     " mm from the 2D origin: CLO3D would create it metres from the avatar "
                                     "(pieces falling and a running simulation have crashed CLO3D). Lay the "
                                     "pieces out near the origin, or pass allow_far: true.")
    thickness = params.get("collision_thickness", 0)
    default_mesh = params.get("particle_distance")
    fabric_names = _fabric_names()
    roles = {}
    for role, value in (params.get("fabrics") or {}).items():
        roles[role] = _resolve_fabric(value, fabric_names)
    do_arrange = params.get("arrange", True) and export_api.GetAvatarCount() > 0
    arr_names = _arrangement_names() if do_arrange else []
    warnings = []
    if params.get("arrange", True) and not do_arrange:
        warnings.append("no avatar loaded: pieces not arranged")

    n0 = pattern_api.GetPatternCount()
    idx, out_pieces = {}, []
    for i, p in enumerate(pieces):
        k = n0 + i
        pattern_api.CreatePatternWithPoints(_points(p["points"]))
        if pattern_api.GetPatternCount() != k + 1:
            raise ValueError("CLO3D did not create piece " + p["name"] + " (count " +
                             str(pattern_api.GetPatternCount()) + ", expected " + str(k + 1) + ")")
        pattern_api.SetPatternPieceName(k, str(p["name"]))
        idx[p["name"]] = k
        rec = {"name": p["name"], "index": k}
        if thickness is not None:
            pattern_api.SetAddlThicknessCollision(k, float(thickness))
        mesh = p.get("particle_distance", default_mesh)
        if mesh is not None:
            pattern_api.SetParticleDistanceOfPattern(k, float(mesh))
            rec["particle_distance"] = float(mesh)
        role = p.get("fabric")
        fab = roles.get(role, roles.get("*"))
        if fab is not None:
            pattern_api.SetPatternPieceFabricIndex(k, int(fab))
            rec["fabric_index"] = int(fab)
        elif role and roles:
            warnings.append(p["name"] + ": no fabric given for role " + str(role))
        if p.get("layer"):
            pattern_api.SetPatternLayer(k, int(p["layer"]))
        for line in p.get("internals") or []:   # roll lines, pocket openings, fold lines
            pattern_api.CreateInternalShapeWithPoints(k, _points(line), False)
        if do_arrange and p.get("arrange"):
            ar = p["arrange"]
            ai, an = _resolve_arrangement(ar, arr_names)
            if ai is None:
                warnings.append(p["name"] + ": arrangement point not found: " + str(ar.get("name")))
                rec["arranged"] = None
            else:
                pattern_api.SetArrangement(k, int(ai))
                if ar.get("x") is not None and ar.get("y") is not None:
                    pattern_api.SetArrangementPosition(k, int(ar["x"]), int(ar["y"]), int(ar.get("offset", 0)))
                if ar.get("orientation") is not None:
                    pattern_api.SetArrangementOrientation(k, int(ar["orientation"]))
                rec["arranged"] = an
        edges = p.get("edges") or []
        if edges:
            got = [pattern_api.GetLineLength(k, j) for j in range(len(edges))]
            rec["lines"] = len(got)
            devs = [abs(float(g) - float(e.get("length_mm", g))) for g, e in zip(got, edges) if "length_mm" in e]
            if devs:
                rec["max_deviation_mm"] = round(max(devs), 2)
        out_pieces.append(rec)

    by_name = {p["name"]: p for p in pieces}
    sewn, failed = 0, []
    for s_ in data.get("seams", []):
        if s_[0] not in by_name or s_[3] not in by_name:
            failed.append({"seam": s_[:6], "error": "piece missing"})
            continue
        try:
            la, da = _garment_edge(by_name[s_[0]], s_[1], s_[2])
            lb, db = _garment_edge(by_name[s_[3]], s_[4], s_[5])
            pattern_api.AddSeamlinePairGroup(idx[s_[0]], la, idx[s_[3]], lb, da, db)
            sewn += 1
        except Exception as e:
            failed.append({"seam": s_[:6], "error": str(e)})
    holds_sewn, holds_failed = 0, []
    for h in data.get("holds", []):
        if h["top"] not in by_name or h["under"] not in by_name:
            holds_failed.append({"hold": h["top"], "error": "piece missing"})
            continue
        try:
            line, fwd = _garment_edge(by_name[h["top"]], h["edge"][0], h["edge"][1])
            a, b = (h["edge"][0], h["edge"][1]) if fwd else (h["edge"][1], h["edge"][0])
            child = pattern_api.CreateInternalShapeWithPoints(idx[h["under"]], _points([h["at"][a], h["at"][b]]), False)
            pattern_api.AddSeamlinePairGroup(idx[h["top"]], line, idx[h["under"]], int(child), 0, True, True)
            if h.get("layer"):
                pattern_api.SetPatternLayer(idx[h["top"]], int(h["layer"]))
            holds_sewn += 1
        except Exception as e:
            holds_failed.append({"hold": h["top"], "error": str(e)})
    unplaced = [p["name"] for p in pieces if not p.get("arrange")] if do_arrange else [p["name"] for p in pieces]
    return {"pieces": out_pieces, "indices": idx, "seams_sewn": sewn, "seams_failed": failed,
            "holds_sewn": holds_sewn, "holds_failed": holds_failed, "unplaced": unplaced, "warnings": warnings}


def handle_get_scene_summary(params):
    """Everything an agent reads before acting, in one call: pieces with their state, fabrics, avatars and
    the arrangement points of the avatar."""
    count = pattern_api.GetPatternCount()
    arr_by_piece = {}
    if export_api.GetAvatarCount():
        for a in _items(pattern_api.GetArrangementOfPattern()):
            d = dict(a) if isinstance(a, dict) else _fields(a)
            if not isinstance(d, dict):
                continue
            k = None
            for key, v in d.items():
                if _norm_key(key) == "patternindex":
                    k = int(v)
            if k is not None:
                arr_by_piece[k] = d
    pieces = []
    for i in range(count):
        a = arr_by_piece.get(i, {})
        arr_name = None
        for key, v in a.items():
            if _norm_key(key) == "arrangementname":
                arr_name = v
        pieces.append({"index": i, "name": pattern_api.GetPatternPieceName(i),
                       "position": _safe(pattern_api.GetPatternPiecePos, i),
                       "bounding_box": _safe(pattern_api.GetBoundingBoxOfPattern, i),
                       "fabric_index": _safe(pattern_api.GetPatternPieceFabricIndex, i),
                       "layer": _safe(pattern_api.GetPatternLayer, i),
                       "particle_distance": _safe(pattern_api.GetParticleDistanceOfPattern, i),
                       "arrangement": arr_name})
    fabrics = [{"index": i, "name": n} for i, n in enumerate(_fabric_names())]
    avatars = handle_get_avatars({})["avatars"]
    names = _arrangement_names() if avatars else []
    return {"project": _safe(utility_api.GetProjectName), "pattern_count": count, "pieces": pieces,
            "fabrics": fabrics, "avatars": avatars,
            "arrangement_points": [{"index": i, "name": n} for i, n in enumerate(names)]}


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
    "save_project": handle_save_file,
    "get_garment_info": handle_get_garment_info,
    "get_pattern_count": handle_get_pattern_count,
    "get_pattern_list": handle_get_pattern_list,
    "get_pattern_info": handle_get_pattern_info,
    "get_pattern_bounding_box": handle_get_bounding_box,
    "set_pattern_name": handle_set_pattern_name,
    "copy_pattern": handle_copy_pattern,
    "delete_pattern": handle_delete_pattern,
    "flip_pattern": handle_flip_pattern,
    "create_pattern": handle_create_pattern,
    "get_arrangement_list": handle_get_arrangement_list,
    "get_fabric_count": handle_get_fabric_count,
    "get_fabric_list": handle_get_fabric_list,
    "add_fabric": handle_add_fabric,
    "assign_fabric_to_pattern": handle_assign_fabric,
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
    "get_line_lengths": handle_get_line_lengths,
    "sew_lines": handle_sew,
    "get_seam_groups": handle_get_seam_groups,
    "arrange_pattern": handle_arrange_pattern,
    "get_pattern_arrangements": handle_get_pattern_arrangements,
    "reset_cloth_arrangement": handle_reset_cloth_arrangement,
    "create_internal_shape": handle_create_internal_shape,
    "convert_shape": handle_convert_shape,
    "offset_as_internal_line": handle_offset_as_internal_line,
    "delete_line": handle_delete_line,
    "delete_point": handle_delete_point,
    "get_pattern_details": handle_get_pattern_details,
    "set_pattern_position": handle_set_pattern_position,
    "set_grain_direction": handle_set_grain_direction,
    "set_particle_distance": handle_set_particle_distance,
    "unfold_pattern": handle_unfold_pattern,
    "set_shirring": handle_set_shirring,
    "set_elastic": handle_set_elastic,
    "export_pattern_json": handle_export_pattern_json,
    "import_pattern_json": handle_import_pattern_json,
    "import_avatar_measurements": handle_import_avatar_measurements,
    "export_pom": handle_export_pom,
    "set_camera_view": handle_set_camera_view,
    "export_view_image": handle_export_view_image,
    "export_custom_view_snapshot": handle_export_custom_view_snapshot,
    "export_rendering_image": handle_export_rendering_image,
    "set_fit_maps": handle_set_fit_maps,
    "describe_clo_api": handle_api_doc,
    "import_graphic_style": handle_import_graphic_style,
    "import_as_graphic": handle_import_as_graphic,
    "get_graphic_styles": handle_get_graphic_styles,
    "set_graphic_style": handle_set_graphic_style,
    "replace_graphic_style_image": handle_replace_graphic_style_image,
    "set_graphic_style_maps": handle_set_graphic_style_maps,
    "get_graphic_style_maps": handle_get_graphic_style_maps,
    "assign_graphic_style": handle_assign_graphic_style,
    "copy_pattern_with_offset": handle_copy_pattern_with_offset,
    "move_pattern_relative": handle_move_pattern_relative,
    "find_pattern": handle_find_pattern,
    "get_pattern_input_information": handle_get_pattern_input_information,
    "set_pattern_category": handle_set_pattern_category,
    "distribute_internal_lines": handle_distribute_internal_lines,
    "add_annotation": handle_add_annotation,
    "edit_annotation": handle_edit_annotation,
    "get_annotations": handle_get_annotations,
    "find_seam_group": handle_find_seam_group,
    "add_seam_topstitch": handle_add_seam_topstitch,
    "import_topstitch_style": handle_import_topstitch_style,
    "get_topstitches": handle_get_topstitches,
    "set_topstitch": handle_set_topstitch,
    "set_topstitch_style_color": handle_set_topstitch_style_color,
    "get_all_stitches": handle_get_all_stitches,
    "set_seam_taping": handle_set_seam_taping,
    "set_pattern_state": handle_set_pattern_state,
    "set_particle_distance_all": handle_set_particle_distance_all,
    "set_shrinkage": handle_set_shrinkage,
    "get_simulation_quality": handle_get_simulation_quality,
    "set_simulation_settings": handle_set_simulation_settings,
    "set_mesh_type": handle_set_mesh_type,
    "get_mesh_counts": handle_get_mesh_counts,
    "change_fabric_with_json": handle_change_fabric_with_json,
    "export_fabric": handle_export_fabric,
    "get_fabric_details": handle_get_fabric_details,
    "set_fabric": handle_set_fabric,
    "find_fabric": handle_find_fabric,
    "set_pattern_fabric": handle_set_pattern_fabric,
    "get_primary_fabric": handle_get_primary_fabric,
    "import_trim": handle_import_trim,
    "export_trim": handle_export_trim,
    "set_trim_style": handle_set_trim_style,
    "set_button_colors": handle_set_button_colors,
    "get_grading_sizes": handle_get_grading_sizes,
    "change_grading_size": handle_change_grading_size,
    "get_grading_paired_avatar": handle_get_grading_paired_avatar,
    "export_garment_information": handle_export_garment_information,
    "import_garment_information": handle_import_garment_information,
    "export_bom": handle_export_bom,
    "import_pose": handle_import_pose,
    "export_pose": handle_export_pose,
    "set_schematic": handle_set_schematic,
    "export_view_images_all_colorways": handle_export_view_images_all_colorways,
    "start_nesting": handle_start_nesting,
    "stop_nesting": handle_stop_nesting,
    "get_nesting_status": handle_get_nesting_status,
    "set_nesting_pattern": handle_set_nesting_pattern,
    "get_colorway_colors": handle_get_colorway_colors,
    "update_colorways": handle_update_colorways,
    "instance_pattern": handle_instance_pattern,
    "symmetry_pattern": handle_symmetry_pattern,
    "unfold_pattern_by_name": handle_unfold_pattern_by_name,
    "get_selected_patterns": handle_get_selected_patterns,
    "select_pattern": handle_select_pattern,
    "get_pin_count": handle_get_pin_count,
    "remove_pins": handle_remove_pins,
    "set_collision_thickness": handle_set_collision_thickness,
    "export_snapshot_2d": handle_export_snapshot_2d,
    "export_zpac": handle_export_zpac,
    "align_avatar_and_garment": handle_align_avatar_and_garment,
    "get_avatar_properties": handle_get_avatar_properties,
    "set_avatar_properties": handle_set_avatar_properties,
    "delete_avatars": handle_delete_avatars,
    "set_avatar_activation": handle_set_avatar_activation,
    "set_avatar_softness": handle_set_avatar_softness,
    "get_topstitch_style_details": handle_get_topstitch_style_details,
    "set_topstitch_style": handle_set_topstitch_style,
    "get_trim_styles": handle_get_trim_styles,
    "find_trim_style": handle_find_trim_style,
    "get_trim_material": handle_get_trim_material,
    "set_trim_material": handle_set_trim_material,
    "set_zipper_style": handle_set_zipper_style,
    "find_api_types": handle_find_api_types,
    "set_current_fabric": handle_set_current_fabric,
    "set_fabric_maps": handle_set_fabric_maps,
    "get_fabric_maps": handle_get_fabric_maps,
    "set_fabric_material": handle_set_fabric_material,
    "set_fabric_displacement": handle_set_fabric_displacement,
    "set_texture_mapping": handle_set_texture_mapping,
    "import_substance": handle_import_substance,
    "set_substance": handle_set_substance,
    "set_fabric_custom_image": handle_set_fabric_custom_image,
    "duplicate_fabric": handle_duplicate_fabric,
    "get_clo_folders": handle_get_clo_folders,
    "find_assets": handle_find_assets,
    "add_graphic_to_pattern": handle_add_graphic_to_pattern,
    "get_graphic_positions": handle_get_graphic_positions,
    "list_clo_api": handle_list_api,
    "build_garment": handle_build_garment,
    "get_scene_summary": handle_get_scene_summary,
}

# Former command names, kept for scripts and extensions written against them.
ALIASES = {"sew": "sew_lines", "api_doc": "describe_clo_api", "list_api": "list_clo_api", "save_file": "save_project", "get_bounding_box": "get_pattern_bounding_box", "assign_fabric": "assign_fabric_to_pattern"}


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


def handle_reload_plugin(params):
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

    handler = HANDLERS.get(ALIASES.get(cmd_type, cmd_type))
    if not handler and DEV:
        handler = {"run_script": handle_run_script, "reload_plugin": handle_reload_plugin}.get(cmd_type)
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


def _handle_one_request():
    """Process the pending request, if any. Returns True if one was handled."""
    if not os.path.exists(REQUEST_FILE):
        return False
    with open(REQUEST_FILE, "r") as f:
        data = f.read()
    try:
        os.remove(REQUEST_FILE)
    except OSError:
        pass
    if not data.strip():
        return False
    response = process_command(data)
    # Write to temp file first, then rename (atomic)
    tmp_file = RESPONSE_FILE + ".tmp"
    with open(tmp_file, "w") as f:
        f.write(response)
    if os.path.exists(RESPONSE_FILE):
        os.remove(RESPONSE_FILE)
    os.rename(tmp_file, RESPONSE_FILE)
    return True


HEARTBEAT_FILE = os.path.join(COMM_DIR, "heartbeat.txt")


def _tick(hwnd, msg, timer_id, time_ms):
    # NOTHING may escape a ctypes timer callback: an exception raised here is printed by ctypes and the next tick
    # may never be scheduled, which is how the channel died silently with a request left unread (2026-10-18).
    try:
        state = getattr(builtins, "_clo3d_mcp", None)
        if not state:
            return
        now = time.time()
        if now - state.get("beat", 0) > 1.0:   # heartbeat: a dead timer is visible as a stale file, not a silent hang
            state["beat"] = now
            try:
                with open(HEARTBEAT_FILE, "w") as f:
                    f.write("%.0f %d" % (now, state.get("handled", 0)))
            except OSError:
                pass
        if state["busy"]:  # a long call (e.g. simulate) may pump messages and re-enter
            return
        state["busy"] = True
        try:
            if _handle_one_request():
                state["handled"] = state.get("handled", 0) + 1
        except Exception as e:
            print("[CLO MCP] Error: " + str(e))
        finally:
            state["busy"] = False
        if state["stop"] and getattr(builtins, "_clo3d_mcp", None) is state:
            stop()
    except BaseException as e:   # noqa: BLE001  deliberately total
        try:
            print("[CLO MCP] tick error (kept alive): " + repr(e))
        except Exception:
            pass


def _prepare_comm_dir():
    if not os.path.exists(COMM_DIR):
        os.makedirs(COMM_DIR)
    for f in (REQUEST_FILE, RESPONSE_FILE):
        if os.path.exists(f):
            try:
                os.remove(f)
            except OSError:
                pass


def start():
    """Start listening. Re-running the script replaces the previous listener."""
    stop(quiet=True)
    _prepare_comm_dir()
    builtins._clo3d_mcp = {"timer_id": None, "callback": None, "busy": False, "stop": False}
    state = builtins._clo3d_mcp

    if sys.platform == "win32" and not _HAVE_TIMER:
        # A blocking loop would freeze CLO3D's whole window, so refuse rather than fall back to it.
        print("[CLO MCP] Not started: ctypes is not available in CLO3D's Python, so the plug-in cannot")
        print("[CLO MCP] listen without freezing CLO3D. Looked beside " + sys.executable + " and its python3x.zip.")
        print("[CLO MCP] Please report this at github.com/Ubani-Studio/clo3d-mcp/issues with your CLO3D version.")
        return

    if sys.platform == "win32" and not os.environ.get("CLO3D_MCP_TIMER"):
        # 2026-10-18: in CLO 2024.1 a timer set from Python never fires its callback (probed on CLO's GUI thread,
        # thread and window timers, zero ticks), and CLO crashed three times (15, 16, 18 Oct) with BEX64 in
        # Qt5Core.dll, the signature of a timer callback thunk freed while Windows still held the timer. Off by
        # default; drive CLO with ala-clo-exec (the Python Editor) instead. CLO3D_MCP_TIMER=1 re-enables it.
        print("[CLO MCP] Timer listener disabled in this build (see ala-clo-exec).")
        return
    if sys.platform == "win32":
        callback = _TIMERPROC(_tick)
        timer_id = _user32.SetTimer(None, 0, int(POLL_INTERVAL * 1000), callback)
        if timer_id:
            state["timer_id"], state["callback"] = timer_id, callback
            print("[CLO MCP] Plugin started")
            print("[CLO MCP] Listening for commands in: " + COMM_DIR)
            print("[CLO MCP] CLO3D stays usable while listening.")
            return
        print("[CLO MCP] Could not create a Windows timer, falling back to a blocking loop.")

    print("[CLO MCP] Plugin started (the Script Editor stays busy while listening)")
    print("[CLO MCP] Listening for commands in: " + COMM_DIR)
    while not state["stop"]:
        try:
            _handle_one_request()
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
