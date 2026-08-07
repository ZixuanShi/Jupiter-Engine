#!/usr/bin/env python3
# macOS ships Python 3.9, where a builtin generic in an annotation is a runtime TypeError.
# This defers annotation evaluation (PEP 563) so modern syntax stays usable.
from __future__ import annotations

import filecmp
import shutil
import sys
from pathlib import Path

# Scripts/ is the import root for utils, and it is no longer this file's own directory.
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from utils import ROOT

ASSETS = ROOT / "Assets"

# Shaders are a build input rather than runtime content: they compile into the backend's own
# library (default.metallib today, Slang later) and the source itself never ships.
EXCLUDED_DIRS = {"Shaders"}
EXCLUDED_NAMES = {".DS_Store"}


def shipped() -> list[Path]:
    """Return every asset that belongs beside the binary, relative to Assets/."""
    paths = (path.relative_to(ASSETS) for path in ASSETS.rglob("*") if path.is_file())
    return [p for p in paths if p.parts[0] not in EXCLUDED_DIRS and p.name not in EXCLUDED_NAMES]


def stage(destination, assets) -> int:
    """Copy each asset that differs from its staged copy. Returns how many were written."""
    written = 0

    for relative in assets:
        source = ASSETS / relative
        target = destination / relative

        if target.exists() and filecmp.cmp(source, target, shallow=False):
            continue

        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
        written += 1

    return written


def prune(destination, keep) -> None:
    """Delete anything staged that the repository no longer ships.

    Not merely tidiness: code signing seals whatever it finds in the bundle, so a renamed or
    relocated asset would otherwise leave the app carrying — and able to load — both copies.
    """
    if not destination.is_dir():
        return

    for path in destination.rglob("*"):
        if path.is_file() and path.relative_to(destination) not in keep:
            path.unlink()


def main():
    if len(sys.argv) != 2:
        print("Usage: py Scripts/Assets/stage_assets.py path/to/staged/Assets")
        sys.exit(1)

    destination = Path(sys.argv[1])
    assets = shipped()

    written = stage(destination, assets)
    prune(destination, set(assets))

    if written:
        print(f"Staged {written} asset(s)")


if __name__ == "__main__":
    main()
