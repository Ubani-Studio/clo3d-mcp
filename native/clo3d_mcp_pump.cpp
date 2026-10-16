// Native pump for the CLO3D MCP plugin.
//
// CLO3D only runs Python while a script is executing, so the Python plugin's
// background thread never gets scheduled and a foreground loop freezes the UI.
// This dylib is loaded by CLO3D as an event plugin. It schedules a timer on
// CLO3D's main dispatch queue (serviced by the Cocoa run loop Qt runs on) and
// on each tick calls clo3d_mcp_plugin.poll_once() in CLO3D's embedded Python.
// All command handling stays in plugin/clo3d_mcp_plugin.py.
//
// CPython is resolved with dlsym from CLO3D's libpython3.11.dylib at runtime,
// so this builds with no Python or Qt headers.

#include <dispatch/dispatch.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "CloEventPlugin.h"

namespace {

typedef int (*PyIsInitializedFn)();
typedef int (*PyGILStateEnsureFn)();
typedef void (*PyGILStateReleaseFn)(int);
typedef int (*PyRunSimpleStringFn)(const char*);

PyIsInitializedFn Py_IsInitialized_ = nullptr;
PyGILStateEnsureFn PyGILState_Ensure_ = nullptr;
PyGILStateReleaseFn PyGILState_Release_ = nullptr;
PyRunSimpleStringFn PyRun_SimpleString_ = nullptr;

dispatch_source_t g_timer = nullptr;
bool g_loaded = false;

const uint64_t kTickNs = 100 * NSEC_PER_MSEC;

// Directory holding clo3d_mcp_plugin.py. Override with CLO3D_MCP_PLUGIN_DIR.
std::string PluginDir()
{
    const char* env = getenv("CLO3D_MCP_PLUGIN_DIR");
    if (env && *env)
        return env;
    const char* home = getenv("HOME");
    return std::string(home ? home : "") + "/clo3d-mcp/plugin";
}

bool ResolvePython()
{
    Py_IsInitialized_ = (PyIsInitializedFn)dlsym(RTLD_DEFAULT, "Py_IsInitialized");
    PyGILState_Ensure_ = (PyGILStateEnsureFn)dlsym(RTLD_DEFAULT, "PyGILState_Ensure");
    PyGILState_Release_ = (PyGILStateReleaseFn)dlsym(RTLD_DEFAULT, "PyGILState_Release");
    PyRun_SimpleString_ = (PyRunSimpleStringFn)dlsym(RTLD_DEFAULT, "PyRun_SimpleString");
    return Py_IsInitialized_ && PyGILState_Ensure_ && PyGILState_Release_ && PyRun_SimpleString_;
}

// Runs on CLO3D's main thread.
void Tick(void*)
{
    if (!Py_IsInitialized_())
        return;  // CLO3D has not started its interpreter yet

    int gil = PyGILState_Ensure_();
    if (!g_loaded) {
        // The builtins flag stops the module from auto-starting its own loop.
        std::string code =
            "import builtins, sys\n"
            "builtins._CLO3D_MCP_NATIVE_PUMP = True\n"
            "_d = " + std::string("r'") + PluginDir() + "'\n"
            "if _d not in sys.path: sys.path.insert(0, _d)\n"
            "import clo3d_mcp_plugin\n"
            "clo3d_mcp_plugin.prepare_comm_dir()\n"
            "print('[CLO MCP] Native pump running')\n";
        if (PyRun_SimpleString_(code.c_str()) == 0) {
            g_loaded = true;
        } else {
            // Import failed (traceback printed to CLO3D's console). Stop
            // rather than retrying every 100 ms.
            fprintf(stderr, "[CLO MCP] Failed to load clo3d_mcp_plugin from %s\n", PluginDir().c_str());
            dispatch_source_cancel(g_timer);
        }
    } else {
        PyRun_SimpleString_("clo3d_mcp_plugin.poll_once()");
    }
    PyGILState_Release_(gil);
}

__attribute__((constructor)) void StartPump()
{
    if (!ResolvePython()) {
        fprintf(stderr, "[CLO MCP] CPython symbols not found; native pump disabled\n");
        return;
    }
    g_timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
    dispatch_source_set_timer(g_timer, dispatch_time(DISPATCH_TIME_NOW, kTickNs), kTickNs, kTickNs / 10);
    dispatch_set_context(g_timer, nullptr);
    dispatch_source_set_event_handler_f(g_timer, Tick);
    dispatch_resume(g_timer);
}

}  // namespace

// CLO3D loads this library as an event plugin; the drop hooks are unused.
namespace CLOAPI {

bool CloEventPlugin::MouseDropEventOn2DView(float, float, const std::unordered_map<std::string, std::vector<std::string>>&)
{
    return false;
}

bool CloEventPlugin::MouseDropEventOn3DView(float, float, const std::unordered_map<std::string, std::vector<std::string>>&)
{
    return false;
}

}  // namespace CLOAPI

extern "C" CLOAPI::CloEventPlugin* Create()
{
    return new CLOAPI::CloEventPlugin();
}
