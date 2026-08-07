// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

// ImageIO is a plain C API, so this needs no .mm -- the same liberty AppleCallbacks.cpp takes
// with Carbon.
#include <ImageIO/ImageIO.h>

export module jpt.TextureLoader;

import jpt.Path;
import jpt.Texture;
import jpt.TypeDefs;
import std;

export namespace jpt
{
    /** Decodes PNG and JPEG to RGBA8 through Apple's ImageIO, so nothing is vendored.

        Returns an empty Texture on any failure, matching LoadObj -- the caller already has to
        handle a file that decoded to nothing. */
    [[nodiscard]] Texture LoadTexture(const Path& path);
}

/** Declared but not exported, so LoadTexture's machinery reads at a glance here while staying
    unreachable by an importer. Function bodies live in TextureLoader.cpp. */
namespace jpt::local
{
    struct CFDeleter
    {
        void operator()(CFTypeRef pRef) const noexcept;
    };

    template<typename T>
    using CFPtr = std::unique_ptr<std::remove_pointer_t<T>, CFDeleter>;

    // R,G,B,unused in memory order, matching MTLPixelFormatRGBA8Unorm. None of these maps
    // carries alpha. Cast through uint32 because clang rejects a bitwise OR between the two
    // different CoreGraphics enums these constants come from.
    constexpr CGBitmapInfo kRGBA8 = static_cast<CGBitmapInfo>(
        static_cast<uint32>(kCGImageAlphaNoneSkipLast) | static_cast<uint32>(kCGBitmapByteOrder32Big));
}
