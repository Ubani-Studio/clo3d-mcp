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

You'll see `[CLO MCP] Server listening on 127.0.0.1:9877` in the console.

### 2. Connect your AI client

**Claude Code:**
```bash
claude mcp add clo3d -- uvx clo3d-mcp
```

**Claude Desktop** (add to `claude_desktop_config.json`):
```json
{
  "mcpServers": {
    "clo3d": {
      "command": "uvx",
      "args": ["clo3d-mcp"]
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

## What you get (33 tools)

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

## How it works

Two pieces:

**The plugin** (`plugin/clo3d_mcp_plugin.py`) runs inside CLO3D. It's a single Python file that opens a TCP socket on port 9877, listens for JSON commands, calls the CLO3D Python API, and sends back results. No external dependencies.

**The MCP server** (`src/clo3d_mcp/`) is a Python package that connects to that socket and exposes everything as MCP tools. Any MCP-compatible client (Claude, Cursor, etc.) can use it.

```
┌─────────────────┐     TCP/JSON      ┌──────────────────┐     CLO3D API     ┌────────┐
│   MCP Server    │ ◄──────────────► │   CLO3D Plugin    │ ◄──────────────► │ CLO3D  │
│   (clo3d-mcp)   │   port 9877      │  (Python script)  │                  │        │
└─────────────────┘                   └──────────────────┘                  └────────┘
```

## Troubleshooting

**"Cannot connect to CLO3D"**: Is CLO3D running? Did you run the plugin script?

**Port 9877 in use**: You might have another instance open. Restart CLO3D.

**Plugin errors**: Check the Script Editor console in CLO3D for Python tracebacks.

**Simulation/export timeout**: These can take a while. The default timeout is 180 seconds.

## Requirements

- CLO3D with Python scripting support
- Python 3.10+
- uv or pip

## Development

```bash
git clone https://github.com/Ubani-Studio/clo3d-mcp.git
cd clo3d-mcp

# Install dev dependencies
uv sync --dev

# Run tests (no CLO3D needed, uses a mock server)
uv run pytest tests/ -v

# Run the MCP server locally
uv run clo3d-mcp
```

## License

MIT. See [LICENSE](LICENSE).

CLO3D SDK headers in `sdk/` are provided by CLO Virtual Fashion for plugin development. You need your own CLO3D license.
