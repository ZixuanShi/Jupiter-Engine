// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"

export module jpt.Graphics.Enums;

export namespace jpt
{
    JPT_ENUM_UINT8(GraphicsAPI, 
        Unknown,
        Vulkan,
        DX12);

    enum class VSyncMode : uint8
    {
        On,
        Fast,     // Fast sync is a feature that allows the GPU to render frames as quickly as possible without waiting for the monitor's refresh rate, reducing input lag.
        Adaptive, // Adaptive VSync dynamically enables or disables VSync based on the frame rate, reducing stutter and tearing.
        Off
    };
}

export JPT_ENUM_TO_STRING(jpt::GraphicsAPI);
