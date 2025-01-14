// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan.Texture.Sampler;

import jpt.Renderer;
import jpt.Renderer_Vulkan;

import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;

namespace jpt::Vulkan
{
	bool TextureSampler_Vulkan::Init()
	{
		Renderer_Vulkan* pRenderer = GetApplication()->GetRenderer<Renderer_Vulkan>();
		PhysicalDevice& physicalDevice = pRenderer->GetPhysicalDevice();
		LogicalDevice& logicalDevice = pRenderer->GetLogicalDevice();

		VkPhysicalDeviceProperties properties = physicalDevice.GetProperties();

		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
		samplerInfo.mipLodBias = 0.0f;

		if (vkCreateSampler(logicalDevice.GetHandle(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create texture sampler");
			return false;
		}

		return true;
	}

	void TextureSampler_Vulkan::Shutdown()
	{
		Renderer_Vulkan* pRenderer = GetApplication()->GetRenderer<Renderer_Vulkan>();
		LogicalDevice& logicalDevice = pRenderer->GetLogicalDevice();

		vkDestroySampler(logicalDevice.GetHandle(), m_sampler, nullptr);
	}
}