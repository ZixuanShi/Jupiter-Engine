// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.DescriptorSet;

import jpt.Graphics.Constants;

import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.DescriptorSetLayout;
import jpt.Vulkan.DescriptorPool;
import jpt.Vulkan.UniformBuffer;

export namespace jpt::Vulkan
{
	/** Descriptor Set is an actual collection of resource bindings:
		- Contains the actual pointers/references to our resources (uniform buffers, textures, etc.)
		- Can be thought of as a "filled out" version of the layout
		- Can be switched efficiently during rendering */
	class DescriptorSet
	{
	private:
		VkDescriptorSet m_descriptorSet = VK_NULL_HANDLE;
		VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;

	public:
		bool Init(const LogicalDevice& logicalDevice, const DescriptorSetLayout& descriptorSetLayout, const DescriptorPool& descriptorPool, const UniformBuffer& uniformBuffer);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkDescriptorSet GetHandle() const { return m_descriptorSet; }
		VkDescriptorSet* GetHandlePtr() { return &m_descriptorSet; }
	};

	bool DescriptorSet::Init(const LogicalDevice& logicalDevice, const DescriptorSetLayout& descriptorSetLayout, const DescriptorPool& descriptorPool, const UniformBuffer& uniformBuffer)
	{
		m_descriptorPool = descriptorPool.GetHandle();

		VkDescriptorSetAllocateInfo descriptorAllocInfo{};
		descriptorAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorAllocInfo.descriptorPool = m_descriptorPool;
		descriptorAllocInfo.descriptorSetCount = 1;
		descriptorAllocInfo.pSetLayouts = descriptorSetLayout.GetHandlePtr();

		if (const VkResult result = vkAllocateDescriptorSets(logicalDevice.GetHandle(), &descriptorAllocInfo, &m_descriptorSet); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to allocate descriptor sets: %d", result);
			return false;
		}

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffer.GetHandle();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_descriptorSet;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(logicalDevice.GetHandle(), 1, &descriptorWrite, 0, nullptr);

		return true;
	}

	void DescriptorSet::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkFreeDescriptorSets(logicalDevice.GetHandle(), m_descriptorPool, 1, &m_descriptorSet);
	}
}