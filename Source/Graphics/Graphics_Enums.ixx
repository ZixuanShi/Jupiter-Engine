// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"

export module jpt.Graphics.Enums;

export namespace jpt
{
    JPT_ENUM_UINT8(Graphics_API, 
        Unknown,
        Vulkan,
        DX12)
}

export JPT_ENUM_TO_STRING(jpt::Graphics_API);