// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Texture.Sampler;

import jpt.Texture.Sampler;

import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;

import jpt.Any;
import jpt.DynamicArray;

export namespace jpt::Vulkan
{
	class TextureSampler_Vulkan final : public TextureSampler
	{
	private:
		VkSampler m_sampler = VK_NULL_HANDLE;
		PhysicalDevice& m_physicalDevice;
		LogicalDevice& m_logicalDevice;

	public:
		TextureSampler_Vulkan(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice);

		virtual bool Init() override;
		virtual void Shutdown() override;

		VkSampler GetHandle() const { return m_sampler; }
	};

	TextureSampler_Vulkan::TextureSampler_Vulkan(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice)
		: m_physicalDevice(physicalDevice)
		, m_logicalDevice(logicalDevice)
	{
	}

	bool TextureSampler_Vulkan::Init()
	{
		VkPhysicalDeviceProperties properties = m_physicalDevice.GetProperties();

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
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		if (vkCreateSampler(m_logicalDevice.GetHandle(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create texture sampler");
			return false;
		}

		return true;
	}

	void TextureSampler_Vulkan::Shutdown()
	{
		vkDestroySampler(m_logicalDevice.GetHandle(), m_sampler, nullptr);
	}
}