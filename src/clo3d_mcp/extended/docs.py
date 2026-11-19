"""Read-only listing and signatures of the CLO Python API."""

from clo3d_mcp.server import _send, mcp


@mcp.tool()
def list_clo_api() -> dict:
    """List the public functions of every CLO Python API module available in this CLO3D build."""
    return _send("list_clo_api")


@mcp.tool()
def describe_clo_api(module: str, functions: list[str]) -> dict:
    """Read the Python signatures of CLO API functions, without calling them.

    Args:
        module: For example "pattern_api", "export_api", "utility_api".
        functions: Function names to describe.
    """
    return _send("describe_clo_api", {"module": module, "functions": functions})


@mcp.tool()
def find_api_types(text: str) -> dict:
    """Find names containing a text in every module loaded in CLO's Python, without calling them.
    Used to locate CLO's option classes, such as ImportExportOption."""
    return _send("find_api_types", {"text": text})
