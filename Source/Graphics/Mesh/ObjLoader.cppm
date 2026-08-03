// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.ObjLoader;

import jpt.Mesh;
import jpt.Path;

export namespace jpt
{
    /** Wavefront OBJ. Reads v/vn and triangulates every f as a fan, which is exact for the
        convex faces exporters emit. Materials, groups and texture coordinates are skipped.

        Returns an empty Mesh on any failure -- the caller already has to handle a file that
        parsed to nothing, so a second error channel would buy nothing. */
    [[nodiscard]] Mesh LoadObj(const Path& path);
}
