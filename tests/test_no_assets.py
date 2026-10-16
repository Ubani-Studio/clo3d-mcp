"""Nothing of the studio's leaves through this repo: no garments, avatars, patterns, scans or renders, and no
paths that name the studio machine. Reads `git ls-files`, so a file forced past .gitignore with `git add -f`
is still caught. Run by the test suite and by the pre-commit hook (scripts/install-hooks.sh)."""

import hashlib
import os
import re
import subprocess

import pytest

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# Code, docs and config only. Everything else (a .zprj, an .avt, a DXF, a PNG, an OBJ) is refused.
ALLOWED = {".py", ".md", ".toml", ".lock", ".json", ".yml", ".yaml", ".txt", ".cfg", ".ini", ".sh", ".ps1", ""}
ALLOWED_NAMES = {"LICENSE", ".gitignore", ".gitattributes", ".python-version"}
# Our own native source (the macOS pump), not CLO SDK samples.
ALLOWED_NATIVE = {".cpp"}
MAX_BYTES = 200_000
# Words that tie a file to the studio machine or its people, held as SHA-256 so this public file does
# not itself publish them. A word is any run of letters, digits, _ . or -; Users/<name> paths are
# checked by the name. Generic paths (/mnt/c/Users, C:\\Program Files) are fine.
PRIVATE_WORDS = {'efcb703de7aa3c1b1d9401d1d900778c93f4e25ef232e4a74076217b1f1bb7a0', '0809009bc72a864538e95dd9f7212e214e72d0069fcced2e9d126e4c0aee9ae0'}
PRIVATE_USERS = {'5ba2f213da3ccc3c40e1391dbfb866cb84e0028c933b81b6f226cd9d2c909cdb'}  # a Windows profile name: public as a name, private as a path
WORD = re.compile(r"[A-Za-z0-9_.-]+")
STUDIO_PATH = re.compile(r"[A-Z]:[\\/]+(Alabo|ComfyUI)[\\/]", re.I)


def _personal(text):
    for w in set(WORD.findall(text.lower())):
        for part in {w, *re.split(r"[_.-]", w)}:
            if part and hashlib.sha256(part.encode()).hexdigest() in PRIVATE_WORDS:
                return "a private name"
    for name in re.findall(r"Users[\\/]+([A-Za-z0-9_.-]+)", text):
        if hashlib.sha256(name.lower().encode()).hexdigest() in PRIVATE_USERS:
            return "a private profile path"
    m = STUDIO_PATH.search(text)
    return m.group(0) if m else None
# Data files a tool writes at run time; tracked copies would carry project paths and names.
STATE = re.compile(r"(^|/)(catalogue\.json|blocks/[^/]+\.json)$")


def tracked():
    out = subprocess.run(["git", "ls-files", "-z"], cwd=ROOT, capture_output=True, check=True).stdout
    return [p for p in out.decode().split("\0") if p]


def faults(paths, read=lambda p: open(os.path.join(ROOT, p), "rb").read()):
    out = []
    for p in paths:
        name = os.path.basename(p)
        ext = os.path.splitext(name)[1].lower()
        native = p.startswith("native/") and ext in ALLOWED_NATIVE
        if name not in ALLOWED_NAMES and ext not in ALLOWED and not native:
            out.append(f"{p}: {ext or 'no extension'} files are never committed here")
            continue
        if STATE.search(p):
            out.append(f"{p}: run-time state, kept on the studio machine")
            continue
        try:
            data = read(p)
        except FileNotFoundError:
            continue  # staged for deletion
        if len(data) > MAX_BYTES and ext != ".lock":
            out.append(f"{p}: {len(data)} bytes; code and docs are never this large")
        hit = _personal(data.decode("utf-8", "replace"))
        if hit:
            out.append(f"{p}: names the studio machine ({hit})")
    return out


def test_nothing_of_the_studio_is_tracked():
    assert faults(tracked()) == []


@pytest.mark.parametrize("path,body", [
    ("twin/ubani_v3.avt", b"x"),
    ("garments/tee.zprj", b"x"),
    ("blocks/ts.dxf", b"x"),
    ("scan.obj", b"x"),
    ("render.png", b"x"),
    ("catalogue.json", b"{}"),
    ("notes.md", b"open D:/" + bytes.fromhex("416c61626f") + b"/twin.avt"),
    ("tools/x.py", b"PATH = '/home/" + bytes.fromhex("737068696e7879") + b"/clo'"),
    ("big.json", b"x" * 300_000),
    ("setup.md", b"copy to C:\\Users\\" + bytes.fromhex("616c61626f") + b"\\AppData"),
])
def test_the_guard_refuses_what_it_exists_for(path, body):
    assert faults([path], read=lambda p: body), path


def test_the_guard_passes_ordinary_code():
    assert faults(["tools/x.py", "README.md"], read=lambda p: b"print('hello')\n") == []
