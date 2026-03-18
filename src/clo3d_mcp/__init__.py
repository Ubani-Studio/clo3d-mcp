"""CLO3D MCP Server — Control CLO3D via the Model Context Protocol."""

__version__ = "0.1.0"


def main():
    """Entry point for the MCP server (used by uvx/pip scripts)."""
    from clo3d_mcp.server import mcp
    mcp.run()
