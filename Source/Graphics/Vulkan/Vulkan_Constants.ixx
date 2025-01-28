// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Constants;

import jpt.Vector2;
import jpt.TVector3;
import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	constexpr VkFormat kFormat = VK_FORMAT_B8G8R8A8_SRGB;
	constexpr VkColorSpaceKHR kColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

	/** 128 bytes limits. Shared across shaders */
	struct alignas(16) PushConstantData
	{
		alignas(16) float32 value;

		//alignas(16) Vec2f positionOffset;
		//alignas(16) Vec3f color;
	};

	static_assert(sizeof(PushConstantData) <= 128, "Push constant data exceeds 128 bytes limit");
}