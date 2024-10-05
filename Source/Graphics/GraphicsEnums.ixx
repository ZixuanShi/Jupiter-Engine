// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Types/Enum.h"
#include "Debugging/Assert.h"

export module jpt.Graphics.Enums;

export namespace jpt::Graphics
{
	JPT_ENUM_UINT8(API, 
		Unknown,
		DirectX12,
		Vulkan)
}