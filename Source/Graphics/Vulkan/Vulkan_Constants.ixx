// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Constants;

export namespace jpt::Vulkan
{
	constexpr VkFormat kFormat = VK_FORMAT_B8G8R8A8_SRGB;
	constexpr VkColorSpaceKHR kColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
}