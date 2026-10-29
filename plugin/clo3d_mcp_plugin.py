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
}


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

    handler = HANDLERS.get(cmd_type)
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
# request and returns, so CLO3D stays responsive. On macOS a CFRunLoopTimer on
# the main run loop does the same. Elsewhere, fall back to a blocking loop (the
# Script Editor stays busy while listening).
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
    # macOS: CLO.app/Contents/Frameworks/python311/lib-dynload holds _ctypes.cpython-311-darwin.so (CLO3D 2026.1)
    import glob
    roots += sorted(glob.glob(os.path.join(os.path.dirname(sys.executable), "..", "Frameworks", "python3*")))
    for root in roots:
        for sub in ("PythonLib", "DLLs", "lib-dynload", ""):
            folder = os.path.join(root, sub) if sub else root
            has_ctypes = os.path.isdir(folder) and any(
                n.startswith("_ctypes.") and n.endswith((".pyd", ".so")) for n in os.listdir(folder))
            if has_ctypes and folder not in sys.path:
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

# macOS: a CFRunLoopTimer on the main run loop. Qt's Cocoa event loop runs that run loop, so CLO3D fires the
# callback on its own main thread between events, the same way the Win32 timer works on Windows.
_HAVE_CF_TIMER = False
if sys.platform == "darwin":
    ctypes = _import_ctypes()
    _HAVE_CF_TIMER = ctypes is not None

if _HAVE_CF_TIMER:
    _cf = ctypes.CDLL("/System/Library/Frameworks/CoreFoundation.framework/CoreFoundation")
    _CFTIMERCALLBACK = ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_void_p)
    _cf.CFAbsoluteTimeGetCurrent.restype = ctypes.c_double
    _cf.CFRunLoopGetMain.restype = ctypes.c_void_p
    _cf.CFRunLoopTimerCreate.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double, ctypes.c_ulong,
                                         ctypes.c_long, _CFTIMERCALLBACK, ctypes.c_void_p]
    _cf.CFRunLoopTimerCreate.restype = ctypes.c_void_p
    _cf.CFRunLoopAddTimer.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p]
    _cf.CFRunLoopTimerInvalidate.argtypes = [ctypes.c_void_p]
    _cf.CFRelease.argtypes = [ctypes.c_void_p]
    _kCFRunLoopCommonModes = ctypes.c_void_p.in_dll(_cf, "kCFRunLoopCommonModes")


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


def _cf_tick(timer, info):
    _tick(None, 0, 0, 0)


def _prepare_comm_dir():
    if not os.path.exists(COMM_DIR):
        os.makedirs(COMM_DIR)
    for f in (REQUEST_FILE, RESPONSE_FILE):
        if os.path.exists(f):
            try:
                os.remove(f)
            except OSError:
                pass


def _note_listener(kind):
    """Record which listener started, so it can be checked from outside while CLO3D's console cannot redraw."""
    try:
        with open(os.path.join(COMM_DIR, "listener.txt"), "w") as f:
            f.write("%s\npython %s\nexecutable %s\n" % (kind, sys.version.split()[0], sys.executable))
    except OSError:
        pass


def start():
    """Start listening. Re-running the script replaces the previous listener."""
    stop(quiet=True)
    _prepare_comm_dir()
    # keep the previous listener's callback alive: freeing a ctypes thunk the OS may still call crashes CLO3D
    builtins._clo3d_mcp_retired = (getattr(builtins, "_clo3d_mcp_retired", []) + [getattr(builtins, "_clo3d_mcp", None)])[-8:]
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

    if _HAVE_CF_TIMER:
        callback = _CFTIMERCALLBACK(_cf_tick)
        timer = _cf.CFRunLoopTimerCreate(None, _cf.CFAbsoluteTimeGetCurrent() + POLL_INTERVAL, POLL_INTERVAL,
                                         0, 0, callback, None)
        if timer:
            _cf.CFRunLoopAddTimer(_cf.CFRunLoopGetMain(), timer, _kCFRunLoopCommonModes)
            state["cf_timer"], state["callback"] = timer, callback
            _note_listener("cf_timer")
            print("[CLO MCP] Plugin started")
            print("[CLO MCP] Listening for commands in: " + COMM_DIR)
            print("[CLO MCP] CLO3D stays usable while listening.")
            return
        print("[CLO MCP] Could not create a run-loop timer, falling back to a blocking loop.")

    _note_listener("blocking (cf_timer available: %s)" % _HAVE_CF_TIMER)
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
    cf_timer = state.get("cf_timer")
    if cf_timer and _HAVE_CF_TIMER:
        _cf.CFRunLoopTimerInvalidate(cf_timer)
        _cf.CFRelease(cf_timer)
        state["cf_timer"] = None
        if not quiet:
            print("[CLO MCP] Plugin stopped")


# Auto-start
if __name__ == "__main__" or IN_CLO3D:
    start()
