# CLO3D API gotchas

Behaviour of CLO3D's Python API that is not in its documentation, seen while building complete garments and
animations through `call_clo_api` on CLO3D 2025.2. Each entry says what happens and what works instead.

## Files and paths

- **Relative paths resolve against CLO3D's install folder**, not your working directory. Use absolute paths.
- **A missing output folder is not created.** OBJ export and `ExportThumbnail3D` then write nothing and still
  return the path; glTF export stops on a modal error dialog (it cannot save the textures), which blocks the
  plug-in until someone closes it. The built-in export tools now create the folder; with `call_clo_api`, create it
  yourself.
- **`ImportZprj(path, ImportZPRJOption{bAppend: false})` opens a project without a dialog.** `ImportFile` on a
  `.zprj` stops at CLO3D's "Open Project" options box.
- **A `.zprj` is self-contained**: textures and graphics are packed inside it (in the `.zpac` inside the zip). A
  project that looks bare on another computer is an older copy or an older CLO3D, not missing files.

## Indexes change on save

- **Saving drops unused fabrics and graphic styles, and the rest are renumbered.** After a save and reload the same
  fabric can have another index. Keep names, not indexes: `fabric_api.GetFabricName(i)`,
  `utility_api.GetGraphicStyleName(i)`. An index that no longer existed was seen to change a different fabric
  instead of failing.

## What lands in exported images

`ExportThumbnail3D` and snapshots draw the 3D view as it is, display tints included. Clear them before exporting:

| Tint | Comes from | Clear with |
|------|-----------|------------|
| light blue | `SetPatternFreeze(i, True)` | `SetPatternFreeze(i, False)` |
| grey | `SetPatternPieceSolidify(i, True)` | `SetPatternPieceSolidify(i, False)` |
| orange | `SetPatternStrengthen(i, True)` | `SetPatternStrengthen(i, False)` |
| lime | a pattern layer above 0 (`SetPatternLayer`) | `SetPatternLayer(i, 0)` |
| purple | selected pieces | `SelectPatternViaName(name, False)` for each of `GetSelectedPattern()` |

## Pattern pieces

- **`MovePatternPoint` fails on curve points** ("invaild point index") and works on corner points. No function
  returns a point's position, so keep your own drafted coordinates if you reshape pieces later; a piece whose
  corners moved but whose curve points did not is distorted, and its seams can drag the garment off the avatar.
- **Solidify keeps the shape a piece has now**, which right after arranging is the arrangement's shape (wrapped
  round the body), not its flat or intended 3D shape. Solidify after draping, not before.
- **`ResetClothArrangement` resets every piece that is not frozen**, solidified ones included.
- **`SetArrangementPosition`'s offset does not move pieces far from the body**: offsets of several hundred mm placed
  them about where small ones did. To get cloth around a large volume, let it hang first and grow the volume
  underneath (see "Static props" below).
- **`GetArrangementList` returns `[]` while the scene has more than one avatar** (for example after importing an
  OBJ as a prop), and arranging by name then fails. Arrange first, then add the extra avatar.
- **A 3D view preset (`SetCamViewPoint`) frames the bounding box of all avatars.** 2 is the front, 3 a
  three-quarter view, 4 the side and 8 the back. A large static avatar moves where the camera looks.

## Static props, sets and draping forms

- **An OBJ becomes a second, motionless avatar** with `ImportOBJ(path, ImportExportOption{ImportObjectType: 0,
  bAdd: True, scale: 1})` (coordinates in mm, Y up). Cloth collides with it: a runway, a hanger, or a temporary
  form to drape over.
- **Avatar collision settings are reachable**: `SetAvatarProperties(i, {"StaticFriction": "0", "KineticFriction":
  "0", "SkinOffsetMM": "12"})`. A frictionless form lets cloth slide into place instead of riding up on it.
- **Cloth cannot be pushed out by a form that appears around it.** To shape a full skirt over a form, let the
  skirt hang, then re-import the form a few millimetres larger at a time with a short simulation between steps,
  with the form running below the floor so no hem hangs under its rim. Steps of about 20 mm let the form pass
  through the cloth; about 3 mm did not.

## Fabrics and graphics

- **Stock fabrics carry a coloured base texture**, and `SetFabricPBRMaterialBaseColor` multiplies with it: a rose
  colour on the stock crepe or georgette came out olive or tan. Set a plain white base texture
  (`SetBaseTextureMapImageGivenFilePath`) to get the exact colour.
- **A texture's physical size follows its pixel count and resolution (PNG `pHYs`)**: an image with four times the
  pixels and no resolution tag tiles four times larger. Tag it with four times the dpi to keep the size.
- **`ReplaceGraphicStyleFromImage(i, path, 0)` resets the style to the new image's own size** (placed graphics
  shrink to specks) and renames it after the file. Restore the size with `SetGraphicStyleDimensions(i, w, h)`.
- **Graphics can carry normal maps** (`SetGraphicNormalMapTexture(i, path)`): beads and cords stop looking
  painted on.

## Animation

- **`SetCurrentAnimationFrame` does not refresh the 3D view**: thumbnails taken after it all show the same frame.
  `MoveAnimationFrame` does.
- **`RunAnimationRecording` can return at once with nothing recorded.** A real recording takes time and
  `GetCurrentAnimationFrame` reaches the end frame; poll it and send nothing else meanwhile.
- **With Play Type "Frame Stepping", stepping frames in order simulates the cloth live**, so frames exported in order
  can look right with no recording at all. Check a recording by jumping to frames out of order.
- **`ExportAnimationVideo` writes nothing without the XviD codec installed**, and reports no error.
- **Importing a `.mtn` opens the Open Motion dialog**, and the call does not return until it is closed
  (`tools/clo_dialog.ps1 -Contains "Move Avatar"` presses OK).

## Long operations

- **`ExportRenderingImage` can keep CLO3D busy for many minutes**, during which the plug-in answers nothing.
- A heavy simulation can outlast the server's 180 s wait; set `CLO3D_MCP_TIMEOUT` (seconds) to wait longer.
