// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.DescriptorSetLayout;

import jpt.Vulkan.LogicalDevice;

export namespace jpt::Vulkan
{
	/** Descriptor Set Layout defines the "interface" or structure of resources your shader expects:
		- It specifies what types of resources (uniform buffers, textures, etc.) will be bound
		- Where they'll be bound (binding numbers)
		- What shader stages can access them
		- How many resources of each type */
	class DescriptorSetLayout
	{
	private:
		VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;

	public:
		bool Init(const LogicalDevice& logicalDevice);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkDescriptorSetLayout GetHandle() const { return m_descriptorSetLayout; }
		const VkDescriptorSetLayout* GetHandlePtr() const { return &m_descriptorSetLayout; }
	};

	bool DescriptorSetLayout::Init(const LogicalDevice& logicalDevice)
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutBinding bindings[] = { uboLayoutBinding, samplerLayoutBinding };

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = JPT_ARRAY_COUNT(bindings);
		layoutInfo.pBindings = bindings;

		if (const VkResult result = vkCreateDescriptorSetLayout(logicalDevice.GetHandle(), &layoutInfo, nullptr, &m_descriptorSetLayout); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create descriptor set layout: %d", result);
			return false;
		}

		return true;
	}

	void DescriptorSetLayout::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyDescriptorSetLayout(logicalDevice.GetHandle(), m_descriptorSetLayout, nullptr);
	}
}