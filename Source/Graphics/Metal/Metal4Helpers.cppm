// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <simd/simd.h>

#endif

export module jpt.Metal4Helpers;

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS

import jpt.LinearColor;
import jpt.Texture;
import jpt.TypeDefs;
import jpt.Vector3;

export namespace jpt
{
    template<typename T>
    void Release(T*& pObject)
    {
        if (pObject)
        {
            pObject->release();
            pObject = nullptr;
        }
    }

    /** Base colour is the only map that carries colour, so it is the only one the sampler may
        decode. Reading roughness or a normal through the sRGB curve bends the value silently. */
    [[nodiscard]] MTL::PixelFormat FormatOf(TextureSlot slot) noexcept;

    [[nodiscard]] uint32 MipLevelCount(uint32 width, uint32 height) noexcept;

    /** A multisampled render target that never leaves the GPU. Memoryless where there is tile
        memory to hold it -- both attachments are cleared, used and resolved inside one pass, so
        neither needs a DRAM allocation. Apple-family only; an Intel Mac has no tile memory. */
    [[nodiscard]] MTL::Texture* NewAttachment(MTL::Device* pDevice, MTL::PixelFormat format,
                                              uint32 pixelWidth, uint32 pixelHeight) noexcept;

    [[nodiscard]] simd_float4 ToFloat4(const Vec3& vector, float32 w) noexcept;
    [[nodiscard]] simd_float4 ToFloat4(const LinearColor& color) noexcept;
    [[nodiscard]] simd_float4 ToFloat4(const LinearColor& color, float32 w) noexcept;
}

#endif // IS_PLATFORM_MACOS || IS_PLATFORM_IOS
