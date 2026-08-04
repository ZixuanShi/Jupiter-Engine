#!/usr/bin/env python3
# macOS ships Python 3.9, where a builtin generic in an annotation is a runtime TypeError.
# This defers annotation evaluation (PEP 563) so modern syntax stays usable.
from __future__ import annotations

import math
import sys
from pathlib import Path

from utils import ROOT

USAGE = "Usage: py Scripts/normalize_mesh.py Assets/Meshes/Model.obj"


def parse(path) -> dict:
    """Read an OBJ into the pieces the engine's loader cares about.

    Mirrors ObjLoader.cpp: positions, texture coordinates, normals, and faces as lists of
    v/vt/vn index triples. Anything else in the file is carried through untouched.
    """
    positions, texCoords, normals, faces = [], [], [], []

    for line in path.read_text().splitlines():
        if line.startswith("v "):
            positions.append([float(f) for f in line.split()[1:4]])
        elif line.startswith("vt "):
            texCoords.append([float(f) for f in line.split()[1:3]])
        elif line.startswith("vn "):
            normals.append([float(f) for f in line.split()[1:4]])
        elif line.startswith("f "):
            faces.append([corner.split("/") for corner in line.split()[1:]])

    return {"positions": positions, "texCoords": texCoords, "normals": normals, "faces": faces}


def bounds(positions) -> tuple:
    """Return the axis-aligned bounding box as (minimum, maximum), each a list of three floats."""
    axes = list(zip(*positions))
    return [min(a) for a in axes], [max(a) for a in axes]


def report(label, mesh) -> None:
    """Print everything the renderer and the OBJ loader can trip over, measured rather than assumed."""
    positions, texCoords, normals, faces = mesh["positions"], mesh["texCoords"], mesh["normals"], mesh["faces"]
    low, high = bounds(positions)

    print(f"  [{label}]")
    print(f"    bbox     X[{low[0]:.4f},{high[0]:.4f}] Y[{low[1]:.4f},{high[1]:.4f}] Z[{low[2]:.4f},{high[2]:.4f}]")
    print(f"    extent   {high[0] - low[0]:.4f} x {high[1] - low[1]:.4f} x {high[2] - low[2]:.4f}")
    print(f"    center   ({(low[0] + high[0]) / 2:.4f}, {(low[1] + high[1]) / 2:.4f}, {(low[2] + high[2]) / 2:.4f})")

    if not texCoords or not normals:
        return

    # A partially normalled file is the silent killer: ObjLoader only generates normals when the
    # file has none at all, so the corners that omitted one keep Vec3::Zero() and the shader's
    # normalize() yields NaN.
    complete = sum(1 for f in faces if all(len(c) == 3 and c[1] and c[2] for c in f))
    triangles = sum(len(f) - 2 for f in faces)
    print(f"    counts   v={len(positions)} vt={len(texCoords)} vn={len(normals)} f={len(faces)} -> {triangles} triangles")
    print(f"    complete v/vt/vn on every corner: {complete}/{len(faces)}")
    print(f"    non-unit vn: {sum(1 for n in normals if abs(math.dist(n, (0, 0, 0)) - 1.0) > 1e-3)}/{len(normals)}")

    agree, degenerate = 0, 0
    for face in faces:
        corners = [(int(c[0]) - 1, int(c[2]) - 1) for c in face]
        for i in range(1, len(corners) - 1):
            a, b, c = corners[0], corners[i], corners[i + 1]
            pa, pb, pc = positions[a[0]], positions[b[0]], positions[c[0]]
            e1 = [pb[j] - pa[j] for j in range(3)]
            e2 = [pc[j] - pa[j] for j in range(3)]
            cross = [e1[1] * e2[2] - e1[2] * e2[1], e1[2] * e2[0] - e1[0] * e2[2], e1[0] * e2[1] - e1[1] * e2[0]]
            if sum(v * v for v in cross) < 1e-20:
                degenerate += 1
                continue
            summed = [sum(normals[t[1]][j] for t in (a, b, c)) for j in range(3)]
            agree += sum(cross[j] * summed[j] for j in range(3)) > 0

    live = triangles - degenerate
    print(f"    winding  {agree}/{live} CCW-consistent with their normals ({100 * agree / live:.2f}%)")
    print(f"    degenerate triangles: {degenerate}")


def normalize(path, mesh) -> bool:
    """Rewrite the file's v lines so the bounding box is centred on the origin at unit max extent.

    Only positions move. A uniform scale plus a translation leaves normals and texture
    coordinates unchanged, which is why vt and vn need no fixup. The scale must stay uniform:
    Mesh.metal transforms the normal by the model matrix rather than its inverse-transpose, so a
    non-uniform one would skew the lighting.

    Returns whether the file was rewritten, so a second run reports itself as a no-op.
    """
    low, high = bounds(mesh["positions"])
    center = [(low[i] + high[i]) / 2 for i in range(3)]
    scale = 1.0 / max(high[i] - low[i] for i in range(3))

    if max(abs(c) for c in center) < 1e-6 and abs(scale - 1.0) < 1e-6:
        return False

    lines, index = path.read_text().splitlines(), 0
    for i, line in enumerate(lines):
        if not line.startswith("v "):
            continue
        x, y, z = ((mesh["positions"][index][j] - center[j]) * scale for j in range(3))
        lines[i] = f"v {x:.6f} {y:.6f} {z:.6f}"
        index += 1

    path.write_text("\n".join(lines) + "\n")
    return True


def main():
    if len(sys.argv) != 2:
        print(USAGE)
        sys.exit(1)

    path = Path(sys.argv[1])
    if not path.is_absolute():
        path = ROOT / path

    if not path.is_file():
        print(f"No such mesh: {path}")
        print(USAGE)
        sys.exit(1)

    print(f"{path.name}")
    mesh = parse(path)
    if not mesh["positions"]:
        print(f"No 'v ' lines found. Is this a Wavefront OBJ?")
        sys.exit(1)

    report("before", mesh)

    if not normalize(path, mesh):
        print("  already normalized, left alone")
        return

    report("after", parse(path))


if __name__ == "__main__":
    main()
