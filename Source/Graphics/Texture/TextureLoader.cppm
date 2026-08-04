// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.TextureLoader;

import jpt.Path;
import jpt.Texture;

export namespace jpt
{
    /** Decodes PNG and JPEG to RGBA8 through Apple's ImageIO, so nothing is vendored.

        Returns an empty Texture on any failure, matching LoadObj -- the caller already has to
        handle a file that decoded to nothing. */
    [[nodiscard]] Texture LoadTexture(const Path& path);
}
