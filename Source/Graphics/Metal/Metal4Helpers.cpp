// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <simd/simd.h>

#include "Graphics/GraphicsConstants.h"

#endif

module jpt.Metal4Helpers;

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS

import std;

namespace jpt
{
    uint32 MipLevelCount(uint32 width, uint32 height) noexcept
    {
        return static_cast<uint32>(std::bit_width(std::max(width, height)));
    }

    MTL::Texture* NewAttachment(MTL::Device* pDevice, MTL::PixelFormat format,
                                uint32 pixelWidth, uint32 pixelHeight) noexcept
    {
        MTL::TextureDescriptor* pDesc = MTL::TextureDescriptor::texture2DDescriptor(format, pixelWidth, pixelHeight, false);

        // The type must change with the sample count; a count alone is rejected.
        pDesc->setTextureType(MTL::TextureType2DMultisample);
        pDesc->setSampleCount(kSampleCount);
        pDesc->setUsage(MTL::TextureUsageRenderTarget);
        pDesc->setStorageMode(pDevice->supportsFamily(MTL::GPUFamilyApple1) ? MTL::StorageModeMemoryless : MTL::StorageModePrivate);

        return pDevice->newTexture(pDesc);
    }

    simd_float4 ToFloat4(const LinearColor& color) noexcept
    {
        return simd_make_float4(color.r, color.g, color.b, color.a);
    }

    simd_float4 ToFloat4(const LinearColor& color, float32 w) noexcept
    {
        return simd_make_float4(color.r, color.g, color.b, w);
    }
}

#endif // IS_PLATFORM_MACOS || IS_PLATFORM_IOS
