"""
CLO3D MCP Plugin: Socket server that runs inside CLO3D.

Drag this file into CLO3D's Script Editor and run it.
Opens a non-blocking TCP socket server on port 9877, listens for JSON
commands from the MCP server, executes CLO3D API calls, returns results.

Protocol:
  Request:  {"id": "uuid", "type": "command_name", "params": {...}}
  Response: {"id": "uuid", "status": "success", "result": {...}}
  Error:    {"id": "uuid", "status": "error", "message": "..."}
"""

import json
import socket
import threading
import time
import traceback
import os
import tempfile

# CLO3D API modules (available when running inside CLO3D)
try:
    import CLO_API_EXT as export_api
    import CLO_API_FAB as fabric_api
    import CLO_API_IMP as import_api
    import CLO_API_PAT as pattern_api
    import CLO_API_UTL as utility_api
    IN_CLO3D = True
except ImportError:
    IN_CLO3D = False
    print("[CLO MCP] WARNING: Not running inside CLO3D. API calls will fail.")

HOST = "127.0.0.1"
PORT = 9877
BUFFER_SIZE = 65536

_server_running = False
_server_thread = None


# ---------------------------------------------------------------------------
# Command handlers
# ---------------------------------------------------------------------------

def handle_ping(params):
    return {"pong": True, "in_clo3d": IN_CLO3D}


# ── Scene ──────────────────────────────────────────────────────────────────

def handle_get_project_info(params):
    name = utility_api.GetProjectName()
    path = utility_api.GetProjectFilePath()
    major = utility_api.GetMajorVersion()
    minor = utility_api.GetMinorVersion()
    patch = utility_api.GetPatchVersion()
    pattern_count = pattern_api.GetPatternCount()
    fabric_count = fabric_api.GetFabricCount()
    colorway_count = utility_api.GetColorwayCount()
    return {
        "project_name": name,
        "project_path": path,
        "clo_version": f"{major}.{minor}.{patch}",
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
    tmp = os.path.join(tempfile.gettempdir(), "clo_garment_info.json")
    export_api.ExportGarmentInformation(tmp)
    if os.path.exists(tmp):
        with open(tmp, "r", encoding="utf-8") as f:
            data = json.load(f)
        os.remove(tmp)
        return {"garment_info": data}
    return {"garment_info": None}


# ── Pattern ────────────────────────────────────────────────────────────────

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
    pattern_api.FlipPatternPiece(index, horizontal)
    return {"flipped": True, "index": index, "horizontal": horizontal}


def handle_create_pattern(params):
    points = params["points"]  # list of [x, y] or [x, y, type]
    point_tuples = []
    for p in points:
        x, y = p[0], p[1]
        vtype = p[2] if len(p) > 2 else 0  # 0=straight, 2=spline, 3=bezier
        point_tuples.append((x, y, vtype))
    result = pattern_api.CreatePatternWithPoints(point_tuples)
    return {"created": True, "point_count": len(point_tuples), "result": result}


def handle_get_arrangement_list(params):
    arr_list = pattern_api.GetArrangementList()
    return {"arrangements": arr_list}


# ── Fabric ─────────────────────────────────────────────────────────────────

def handle_get_fabric_count(params):
    count = fabric_api.GetFabricCount()
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


def handle_replace_fabric(params):
    fabric_index = params["fabric_index"]
    file_path = params["file_path"]
    result = fabric_api.ReplaceFabric(fabric_index, file_path)
    return {"replaced": result, "fabric_index": fabric_index}


def handle_assign_fabric(params):
    fabric_index = params["fabric_index"]
    pattern_index = params["pattern_index"]
    option = params.get("assign_option", 1)  # 1=current colorway, 2=all unlinked, 3=all linked
    result = fabric_api.AssignFabricToPattern(fabric_index, pattern_index, option)
    return {"assigned": result, "fabric_index": fabric_index, "pattern_index": pattern_index}


def handle_set_fabric_color(params):
    fabric_index = params["fabric_index"]
    r = params.get("r", 255)
    g = params.get("g", 255)
    b = params.get("b", 255)
    fabric_api.SetFabricPBRMaterialBaseColor(fabric_index, r, g, b)
    return {"set": True, "fabric_index": fabric_index, "color": [r, g, b]}


def handle_get_fabric_for_pattern(params):
    pattern_index = params["pattern_index"]
    fabric_index = fabric_api.GetFabricIndexForPattern(pattern_index)
    return {"pattern_index": pattern_index, "fabric_index": fabric_index}


def handle_delete_fabric(params):
    fabric_index = params["fabric_index"]
    result = fabric_api.DeleteFabric(fabric_index)
    return {"deleted": result, "fabric_index": fabric_index}


# ── Export ─────────────────────────────────────────────────────────────────

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
    return {"exported": bool(result), "file_path": result or file_path, "format": "obj"}


def handle_export_fbx(params):
    file_path = params["file_path"]
    result = export_api.ExportFBX(file_path)
    return {"exported": bool(result), "file_path": result or file_path, "format": "fbx"}


def handle_export_glb(params):
    file_path = params["file_path"]
    options = params.get("options", {})
    if options:
        opt = export_api.NewImportExportOption()
        for key, val in options.items():
            if hasattr(opt, key):
                setattr(opt, key, val)
        result = export_api.ExportGLB(file_path, opt)
    else:
        result = export_api.ExportGLB(file_path)
    return {"exported": bool(result), "file_path": result or file_path, "format": "glb"}


def handle_export_gltf(params):
    file_path = params["file_path"]
    options = params.get("options", {})
    if options:
        opt = export_api.NewImportExportOption()
        for key, val in options.items():
            if hasattr(opt, key):
                setattr(opt, key, val)
        result = export_api.ExportGLTF(file_path, opt)
    else:
        result = export_api.ExportGLTF(file_path)
    return {"exported": bool(result), "file_path": result or file_path, "format": "gltf"}


def handle_export_thumbnail(params):
    file_path = params["file_path"]
    width = params.get("width", 512)
    height = params.get("height", 512)
    result = export_api.ExportThumbnail3D(file_path, width, height)
    return {"exported": bool(result), "file_path": result or file_path}


def handle_export_snapshot(params):
    file_path = params["file_path"]
    result = export_api.ExportSnapshot3D(file_path)
    return {"exported": bool(result), "file_path": result or file_path}


def handle_export_turntable(params):
    file_path = params["file_path"]
    result = export_api.ExportTurntableImages(file_path)
    return {"exported": bool(result), "file_path": result or file_path}


def handle_export_tech_pack(params):
    file_path = params["file_path"]
    result = export_api.ExportTechPack(file_path)
    return {"exported": bool(result), "file_path": result or file_path}


# ── Import ─────────────────────────────────────────────────────────────────

def handle_import_file(params):
    file_path = params["file_path"]
    result = import_api.ImportFile(file_path)
    return {"imported": result, "file_path": file_path}


def handle_import_avatar(params):
    file_path = params["file_path"]
    apf_path = params.get("apf_path", "")
    result = import_api.ImportAVAC(file_path, apf_path)
    return {"imported": result, "file_path": file_path}


def handle_import_fabric(params):
    file_path = params["file_path"]
    index = fabric_api.AddFabric(file_path)
    return {"imported": True, "fabric_index": index, "file_path": file_path}


# ── Simulation ─────────────────────────────────────────────────────────────

def handle_simulate(params):
    steps = params.get("steps", 100)
    utility_api.Simulate(steps)
    return {"simulated": True, "steps": steps}


def handle_set_simulation_quality(params):
    quality = params["quality"]  # integer quality level
    utility_api.SetSimulationQuality(quality)
    return {"quality": quality}


# ── Colorway ───────────────────────────────────────────────────────────────

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
    index = params["colorway_index"]
    utility_api.CopyColorway(index)
    return {"copied": True, "source_index": index}


def handle_delete_colorway(params):
    index = params["colorway_index"]
    utility_api.DeleteColorwayItem(index)
    return {"deleted": True, "colorway_index": index}


# ── Avatar ─────────────────────────────────────────────────────────────────

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

HANDLERS = {
    # Internal
    "ping": handle_ping,
    # Scene
    "get_project_info": handle_get_project_info,
    "new_project": handle_new_project,
    "open_file": handle_open_file,
    "save_file": handle_save_file,
    "get_garment_info": handle_get_garment_info,
    # Pattern
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
    # Fabric
    "get_fabric_count": handle_get_fabric_count,
    "get_fabric_list": handle_get_fabric_list,
    "add_fabric": handle_add_fabric,
    "replace_fabric": handle_replace_fabric,
    "assign_fabric": handle_assign_fabric,
    "set_fabric_color": handle_set_fabric_color,
    "get_fabric_for_pattern": handle_get_fabric_for_pattern,
    "delete_fabric": handle_delete_fabric,
    # Export
    "export_obj": handle_export_obj,
    "export_fbx": handle_export_fbx,
    "export_glb": handle_export_glb,
    "export_gltf": handle_export_gltf,
    "export_thumbnail": handle_export_thumbnail,
    "export_snapshot": handle_export_snapshot,
    "export_turntable": handle_export_turntable,
    "export_tech_pack": handle_export_tech_pack,
    # Import
    "import_file": handle_import_file,
    "import_avatar": handle_import_avatar,
    "import_fabric": handle_import_fabric,
    # Simulation
    "simulate": handle_simulate,
    "set_simulation_quality": handle_set_simulation_quality,
    # Colorway
    "get_colorways": handle_get_colorways,
    "set_current_colorway": handle_set_current_colorway,
    "set_colorway_name": handle_set_colorway_name,
    "copy_colorway": handle_copy_colorway,
    "delete_colorway": handle_delete_colorway,
    # Avatar
    "get_avatars": handle_get_avatars,
    "show_hide_avatar": handle_show_hide_avatar,
    "get_avatar_genders": handle_get_avatar_genders,
}


# ---------------------------------------------------------------------------
# Socket server
# ---------------------------------------------------------------------------

def process_command(data):
    """Parse and execute a single JSON command."""
    try:
        request = json.loads(data)
    except json.JSONDecodeError as e:
        return json.dumps({"id": None, "status": "error", "message": f"Invalid JSON: {e}"})

    req_id = request.get("id")
    cmd_type = request.get("type")
    params = request.get("params", {})

    handler = HANDLERS.get(cmd_type)
    if not handler:
        return json.dumps({
            "id": req_id,
            "status": "error",
            "message": f"Unknown command: {cmd_type}",
        })

    try:
        result = handler(params)
        return json.dumps({"id": req_id, "status": "success", "result": result})
    except Exception as e:
        return json.dumps({
            "id": req_id,
            "status": "error",
            "message": str(e),
            "traceback": traceback.format_exc(),
        })


def server_loop():
    """Non-blocking socket server loop running on a background thread."""
    global _server_running

    server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_sock.settimeout(1.0)  # 1s timeout for accept()

    try:
        server_sock.bind((HOST, PORT))
        server_sock.listen(1)
        print(f"[CLO MCP] Server listening on {HOST}:{PORT}")
    except OSError as e:
        print(f"[CLO MCP] Failed to bind: {e}")
        _server_running = False
        return

    client = None
    buffer = b""

    while _server_running:
        # Accept new connections
        if client is None:
            try:
                client, addr = server_sock.accept()
                client.setblocking(False)
                buffer = b""
                print(f"[CLO MCP] Client connected from {addr}")
            except socket.timeout:
                continue
            except OSError:
                continue

        # Read data from client
        try:
            chunk = client.recv(BUFFER_SIZE)
            if not chunk:
                print("[CLO MCP] Client disconnected")
                client.close()
                client = None
                continue
            buffer += chunk
        except BlockingIOError:
            pass
        except (ConnectionResetError, BrokenPipeError, OSError):
            print("[CLO MCP] Client connection lost")
            try:
                client.close()
            except Exception:
                pass
            client = None
            continue

        # Process complete messages (newline-delimited JSON)
        while b"\n" in buffer:
            line, buffer = buffer.split(b"\n", 1)
            if not line.strip():
                continue

            response = process_command(line.decode("utf-8", errors="replace"))

            try:
                client.sendall((response + "\n").encode("utf-8"))
            except (BrokenPipeError, ConnectionResetError, OSError):
                print("[CLO MCP] Failed to send response, client lost")
                try:
                    client.close()
                except Exception:
                    pass
                client = None
                break

        time.sleep(0.01)

    # Cleanup
    if client:
        try:
            client.close()
        except Exception:
            pass
    server_sock.close()
    print("[CLO MCP] Server stopped")


# ---------------------------------------------------------------------------
# Public API
# ---------------------------------------------------------------------------

def start():
    """Start the MCP plugin socket server."""
    global _server_running, _server_thread

    if _server_running:
        print("[CLO MCP] Server already running")
        return

    _server_running = True
    _server_thread = threading.Thread(target=server_loop, daemon=True)
    _server_thread.start()
    print("[CLO MCP] Plugin started")


def stop():
    """Stop the MCP plugin socket server."""
    global _server_running, _server_thread

    if not _server_running:
        print("[CLO MCP] Server not running")
        return

    _server_running = False
    if _server_thread:
        _server_thread.join(timeout=5)
        _server_thread = None
    print("[CLO MCP] Plugin stopped")


# Auto-start when loaded in CLO3D
if __name__ == "__main__" or IN_CLO3D:
    start()
