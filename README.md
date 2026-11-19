# CLO3D MCP Server

Talk to CLO3D from Claude, Cursor, or any MCP client. Create patterns, swap fabrics, run simulations, export models. All from a chat window.

If you use CLO3D and you've wished you could script repetitive tasks without digging through the API docs, this is for you.

```
Your AI assistant  <-->  MCP Server  <-->  CLO3D Plugin  <-->  CLO3D
```

## Quick Start

### 1. Load the plugin in CLO3D

One Python file, no dependencies.

1. Open CLO3D
2. **Script > Script Editor**
3. Open `plugin/clo3d_mcp_plugin.py`
4. Hit **Run**

You'll see `[CLO MCP] Plugin started` and `[CLO MCP] Listening for commands in: ...` in the console.

Run it once per CLO3D session. On Windows the plugin listens through a native timer driven by CLO3D's own event loop, so CLO3D stays fully usable. On other platforms it falls back to a blocking loop: the Script Editor stays busy while the plugin listens. Re-running the script replaces the previous listener.

### 2. Connect your AI client

**Claude Code:**
```bash
claude mcp add clo3d -- uvx --from git+https://github.com/Ubani-Studio/clo3d-mcp clo3d-mcp
```

**Claude Desktop** (add to `claude_desktop_config.json`):
```json
{
  "mcpServers": {
    "clo3d": {
      "command": "uvx",
      "args": ["--from", "git+https://github.com/Ubani-Studio/clo3d-mcp", "clo3d-mcp"]
    }
  }
}
```

### 3. Go

Ask things like:
- *"What patterns are in this project?"*
- *"Set the front bodice fabric to red"*
- *"Run 100 steps of simulation then export as GLB"*
- *"Export a turntable of all colorways"*

## What you get (175 tools)

The 46 tools below cover the scene, patterns, fabrics, export, import, simulation and colorways. The 128 extended tools that follow them (one typed plug-in command each, no generic call) cover seams, internal lines, arrangement on the avatar, piece states, mesh, fabrics and textures, graphics, trims, buttons, zippers, avatars and poses, grading, garment information, bill of materials, views, fit maps, technical drawings, nesting, the CLO API documentation, and a whole garment built in one call. Every tool is listed in [docs/TOOLS.md](docs/TOOLS.md).

### Scene
| Tool | What it does |
|------|-------------|
| `get_project_info` | Project name, file path, CLO version, piece counts |
| `new_project` | Start fresh |
| `open_file` | Open .zprj, .zpac, .obj, .fbx, .avt |
| `save_project` | Save as .zprj |
| `get_garment_info` | Dump full garment metadata to JSON |

### Patterns
| Tool | What it does |
|------|-------------|
| `get_pattern_count` | How many pieces |
| `get_pattern_list` | All pieces with names and indices |
| `get_pattern_info` | Full detail on one piece |
| `get_pattern_bounding_box` | Width and height |
| `set_pattern_name` | Rename a piece |
| `copy_pattern` | Duplicate at a given position |
| `delete_pattern` | Remove a piece |
| `flip_pattern` | Flip horizontal or vertical |
| `create_pattern` | Build a new piece from vertex points |
| `get_arrangement_list` | Get avatar arrangement points |

### Fabrics
| Tool | What it does |
|------|-------------|
| `get_fabric_list` | All fabrics with indices |
| `add_fabric` | Load a .zfab or .jfab |
| `replace_fabric` | Swap an existing fabric |
| `assign_fabric_to_pattern` | Put a fabric on a pattern piece |
| `set_fabric_color` | Set PBR base color (RGB) |
| `get_fabric_for_pattern` | Check which fabric is on a piece |

### Export
| Tool | What it does |
|------|-------------|
| `export_obj` | OBJ with options |
| `export_fbx` | FBX |
| `export_glb` | GLB (binary glTF) |
| `export_gltf` | glTF |
| `export_thumbnail` | 3D viewport screenshot |
| `export_snapshot` | Multi-view snapshots |
| `export_turntable` | 360 turntable image sequence |
| `export_tech_pack` | Tech pack with JSON + images |

### Import
| Tool | What it does |
|------|-------------|
| `import_file` | Auto-detect and import any supported format |

### Simulation
| Tool | What it does |
|------|-------------|
| `simulate` | Run cloth sim for N steps |

### Colorways
| Tool | What it does |
|------|-------------|
| `get_colorways` | List all colorways |
| `set_current_colorway` | Switch the active colorway |

### Extended tools (128)

| Area | Tools | Examples |
|------|-------|----------|
| Geometry | 24 | `sew_lines`, `create_internal_shape`, `get_line_lengths`, `unfold_pattern`, `symmetry_pattern`, `instance_pattern`, annotations, `get_scene_summary` (the whole scene in one call) |
| Sewing | 14 | topstitches and their styles, seam tape, shirring, elastic, seam groups |
| Simulation | 14 | `arrange_pattern` on the avatar's arrangement points, layers, freeze and lock, collision thickness, mesh size, shrinkage, advanced settings |
| Materials | 36 | fabric textures and PBR settings, fabric duplication (sewn patches), graphics, trims, buttons, zippers |
| Avatar | 12 | avatar properties and softness, measurements CSV, poses, CLO's own grading sizes |
| Files | 10 | `build_garment` (a garment JSON built in one call: pieces, seams, fabrics by role, placement by point name), pattern JSON, garment information, bill of materials, CLO asset library |
| Visuals | 10 | camera, view images, 2D pattern snapshot, stress and strain maps, technical drawing, colorway images |
| Nesting | 4 | automatic layout on the fabric width |
| Docs | 3 | list and describe the CLO Python API, read-only |

Plug-in command names are the MCP tool names. The former names of six commands (`sew`, `save_file`, `get_bounding_box`, `assign_fabric`, `list_api`, `api_doc`) still work as aliases, so extensions written against them keep running.

Known limits of the CLO3D Python API (2025.2): option classes (`ImportExportOption`, `ImportZPRJOption`, `CloApiRgb`) are not exposed, so opening a project without a dialog, measurements with a given avatar and colorway colours are out of reach; `import_pattern_json` replaces the whole scene; a placed graphic cannot be moved or deleted (use a sewn patch: `duplicate_fabric`, `set_fabric_maps`, `create_internal_shape`, `sew_lines` with `child_b`).

## Pattern making

[pattern-making-skill](https://github.com/adriendelmas/pattern-making-skill) is an agent skill that drafts garment patterns from body measurements, photos or sketches (blocks, dart manipulation, whole jackets and coats from a JSON spec, grading, SVG and DXF) and writes a garment file for this server. `build_garment` builds that file in CLO3D in one call: pieces with exact Bezier edges, seams by node name, fabric by role, mesh size per piece, arrangement by point name, wrap holds. The skill then checks the line lengths, captures the views, simulates and reads the fit maps through the other tools.

## How it works

Two pieces:

**The plugin** (`plugin/clo3d_mcp_plugin.py`) runs inside CLO3D. It's a single Python file that polls a shared temp directory for JSON commands, calls the CLO3D Python API, and writes results back. No external dependencies, no sockets, works with CLO3D's built-in Python.

**The MCP server** (`src/clo3d_mcp/`) is a Python package that writes commands to that directory and reads responses. Any MCP-compatible client (Claude, Cursor, etc.) can use it.

```
┌─────────────────┐   file-based IPC  ┌──────────────────┐     CLO3D API     ┌────────┐
│   MCP Server    │ ◄──────────────► │   CLO3D Plugin    │ ◄──────────────► │ CLO3D  │
│   (clo3d-mcp)   │  %TEMP%/clo3d_mcp │  (Python script)  │                  │        │
└─────────────────┘                   └──────────────────┘                  └────────┘
```

## Your own tools

Keep house tools (your blocks, your naming scheme, your grading rules) in your own folder, private if you like, and the server loads them beside the built-ins. No fork.

```python
# my_tools/house.py
def register(ext):
    @ext.tool()
    def prefix_pieces(prefix: str) -> dict:
        """Prefix every pattern piece name."""
        pieces = ext.send("get_pattern_list")["patterns"]
        for p in pieces:
            ext.send("set_pattern_name", {"pattern_index": p["index"], "name": f"{prefix}-{p['name']}"})
        return {"renamed": len(pieces)}
```

Point the server at the folder:

```bash
claude mcp add clo3d -e CLO3D_MCP_EXTENSIONS=/path/to/my_tools -- uvx --from git+https://github.com/Ubani-Studio/clo3d-mcp clo3d-mcp
```

Several folders or files are separated by `:` (`;` on Windows). Files starting with `_` are skipped, so helpers can sit beside the tools. `ext.send` uses the commands the plug-in already handles. A file that fails to load is named in the server's log and by the `list_extensions` tool, never skipped in silence, and an extension cannot replace a built-in tool.

## Troubleshooting

**"Cannot find CLO3D communication directory"**: Is CLO3D running? Did you run the plugin script?

**Plugin errors**: Check the Script Editor console in CLO3D for Python tracebacks.

**Every call times out although the plugin says it started**: CLO3D's embedded Python does not run background threads once a script returns. Make sure you run the current plugin, which does not rely on a background thread.

**Simulation/export timeout**: These can take a while. The default timeout is 180 seconds.

**Custom comm directory**: Set the `CLO3D_MCP_DIR` environment variable to override the default shared directory path.

## Requirements

- CLO3D with Python scripting support
- Python 3.10+
- uv or pip

## Testing

clo3d-mcp is tested by the people who use it, on the CLO3D versions they run. If you try it, working or not, file a
[Tested on](https://github.com/Ubani-Studio/clo3d-mcp/issues/new?template=tested-on.yml) report: your CLO3D version, OS, client
and what happened. It takes two minutes, failures count as much as successes, and every tester is credited in the next release.

Questions, ideas and polls on what to build next live in [Discussions](https://github.com/Ubani-Studio/clo3d-mcp/discussions).

## Development

```bash
git clone https://github.com/Ubani-Studio/clo3d-mcp.git
cd clo3d-mcp

# Install dev dependencies
uv sync --dev

# Run tests (no CLO3D needed, uses a mock server; every plug-in handler runs against a fake CLO3D API,
# and tests/test_static.py holds every MCP tool to the plug-in command of its own name)
uv run pytest tests/ -v

# Run the MCP server locally
uv run clo3d-mcp
```

## License

MIT for the code authored here. See [LICENSE](LICENSE).

This repository contains no CLO3D SDK material. The plugin uses only the Python API that ships inside CLO3D, so you need your own CLO3D licence and nothing else from CLO.
