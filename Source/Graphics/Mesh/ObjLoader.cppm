// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.ObjLoader;

import jpt.Mesh;
import jpt.Path;

export namespace jpt
{
    /** Wavefront OBJ. Reads v/vt/vn -- including the `v x y z r g b` vertex-colour extension,
        whose values are taken as linear -- and triangulates every f as a fan, which is exact
        for the convex faces exporters emit. Materials and groups are skipped.

        Returns an empty Mesh on any failure -- the caller already has to handle a file that
        parsed to nothing, so a second error channel would buy nothing. */
    [[nodiscard]] Mesh LoadObj(const Path& path);
}
