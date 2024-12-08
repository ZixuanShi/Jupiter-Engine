// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.DescriptorSetLayout;

import jpt.Vulkan.LogicalDevice;

export namespace jpt::Vulkan
{
	class DescriptorSetLayout
	{
	private:
		VkDescriptorSetLayout m_descriptorSetLayout;

	public:
		bool Init(const LogicalDevice& logicalDevice);
		void Shutdown(const LogicalDevice& logicalDevice);
	};

	bool DescriptorSetLayout::Init(const LogicalDevice& logicalDevice)
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboLayoutBinding;

		if (const VkResult result = vkCreateDescriptorSetLayout(logicalDevice.Get(), &layoutInfo, nullptr, &m_descriptorSetLayout); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create descriptor set layout! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	void DescriptorSetLayout::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyDescriptorSetLayout(logicalDevice.Get(), m_descriptorSetLayout, nullptr);
	}
}