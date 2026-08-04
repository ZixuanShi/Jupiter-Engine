// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

// ImageIO is a plain C API, so this needs no .mm -- the same liberty AppleCallbacks.cpp takes
// with Carbon. A GMF is per-file, hence the includes here rather than in the interface unit.
#include <ImageIO/ImageIO.h>

module jpt.TextureLoader;

import jpt.Color;
import jpt.Logger;
import jpt.TypeDefs;
import std;

namespace jpt
{
    namespace
    {
        struct CFDeleter
        {
            void operator()(CFTypeRef pRef) const noexcept { CFRelease(pRef); }
        };

        template<typename T>
        using CFPtr = std::unique_ptr<std::remove_pointer_t<T>, CFDeleter>;

        // R,G,B,unused in memory order, matching MTLPixelFormatRGBA8Unorm. None of these maps
        // carries alpha. Cast through uint32 because clang rejects a bitwise OR between the two
        // different CoreGraphics enums these constants come from.
        constexpr CGBitmapInfo kRGBA8 = static_cast<CGBitmapInfo>(
            static_cast<uint32>(kCGImageAlphaNoneSkipLast) | static_cast<uint32>(kCGBitmapByteOrder32Big));
    }

    Texture LoadTexture(const Path& path)
    {
        const std::string file = path.GetAbsolute().string();

        const CFPtr<CFURLRef> url{ CFURLCreateFromFileSystemRepresentation(
            nullptr, reinterpret_cast<const UInt8*>(file.c_str()), file.size(), false) };
        if (!url)
        {
            return {};
        }

        const CFPtr<CGImageSourceRef> source{ CGImageSourceCreateWithURL(url.get(), nullptr) };
        if (!source)
        {
            Debug::Error("Cannot open image: {}", file);
            return {};
        }

        const CFPtr<CGImageRef> decoded{ CGImageSourceCreateImageAtIndex(source.get(), 0, nullptr) };
        if (!decoded)
        {
            Debug::Error("Cannot decode image: {}", file);
            return {};
        }

        const uint32 width  = static_cast<uint32>(CGImageGetWidth(decoded.get()));
        const uint32 height = static_cast<uint32>(CGImageGetHeight(decoded.get()));

        Texture texture;
        texture.Resize(width, height);

        // sRGB rather than device RGB: every one of these files is sRGB-tagged, so drawing into a
        // matching space is a copy rather than a colour conversion, and the data maps -- roughness,
        // metallic, AO -- reach the GPU byte-exact. Whether their values are *read* as sRGB is the
        // texture format's job, not this one's.
        const CFPtr<CGColorSpaceRef> colorSpace{ CGColorSpaceCreateWithName(kCGColorSpaceSRGB) };
        const CFPtr<CGContextRef> context{ CGBitmapContextCreate(
            texture.Data(), width, height, 8, texture.RowPitch(), colorSpace.get(), kRGBA8) };

        if (!context)
        {
            Debug::Error("Cannot create a bitmap context for {}", file);
            return {};
        }

        CGContextDrawImage(context.get(), CGRectMake(0.0, 0.0, width, height), decoded.get());

        Debug::Info("Loaded {}: {}x{}", path.GetFileName(), width, height);
        return texture;
    }
}
