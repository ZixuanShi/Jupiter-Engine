// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

// A GMF is per-file, hence the include again rather than inherited from the interface unit.
#include <ImageIO/ImageIO.h>

module jpt.TextureLoader;

import jpt.Color;
import jpt.Logger;
import jpt.TypeDefs;
import std;

namespace jpt
{
    Texture LoadTexture(const Path& path)
    {
        const std::string file = path.GetAbsolute().string();

        const local::CFPtr<CFURLRef> url{ CFURLCreateFromFileSystemRepresentation(
            nullptr, reinterpret_cast<const UInt8*>(file.c_str()), file.size(), false) };
        if (!url)
        {
            return {};
        }

        const local::CFPtr<CGImageSourceRef> source{ CGImageSourceCreateWithURL(url.get(), nullptr) };
        if (!source)
        {
            Debug::Error("Cannot open image: {}", file);
            return {};
        }

        const local::CFPtr<CGImageRef> decoded{ CGImageSourceCreateImageAtIndex(source.get(), 0, nullptr) };
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
        const local::CFPtr<CGColorSpaceRef> colorSpace{ CGColorSpaceCreateWithName(kCGColorSpaceSRGB) };
        const local::CFPtr<CGContextRef> context{ CGBitmapContextCreate(
            texture.Data(), width, height, 8, texture.RowPitch(), colorSpace.get(), local::kRGBA8) };

        if (!context)
        {
            Debug::Error("Cannot create a bitmap context for {}", file);
            return {};
        }

        CGContextDrawImage(context.get(), CGRectMake(0.0, 0.0, width, height), decoded.get());

        Debug::Info("Loaded {}: {}x{}", path.GetFileName(), width, height);
        return texture;
    }

    namespace local
    {
        void CFDeleter::operator()(CFTypeRef pRef) const noexcept
        {
            CFRelease(pRef);
        }
    }
}
