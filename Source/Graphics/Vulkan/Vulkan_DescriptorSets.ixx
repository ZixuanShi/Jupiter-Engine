// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.DescriptorSets;

import jpt.Graphics.Constants;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.DescriptorPool;
import jpt.Vulkan.DescriptorSetLayout;
import jpt.Vulkan.UniformBuffer;
import jpt.Vulkan.UniformBufferObject;

import jpt.TypeDefs;
import jpt.StaticArray;

export namespace jpt::Vulkan
{
	class DescriptorSets
	{
	private:
		StaticArray<VkDescriptorSet, kMaxFramesInFlight> m_sets;

	public:
		bool Init(const LogicalDevice& logicalDevice, 
			const DescriptorSetLayout& descriptorSetLayout, const DescriptorPool& descriptorPool, 
			const StaticArray<UniformBuffer, kMaxFramesInFlight>& uniformBuffers);

		VkDescriptorSet* GetPtr(size_t index) { return &m_sets[index]; }
	};

	bool DescriptorSets::Init(const LogicalDevice& logicalDevice, 
		const DescriptorSetLayout& descriptorSetLayout, const DescriptorPool& descriptorPool, 
		const StaticArray<UniformBuffer, kMaxFramesInFlight>& uniformBuffers)
	{
		StaticArray<VkDescriptorSetLayout, kMaxFramesInFlight> descriptorSetLayouts;
		for (size_t i = 0; i < kMaxFramesInFlight; ++i)
		{
			descriptorSetLayouts[i] = descriptorSetLayout.Get();
		}

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool.Get();
		allocInfo.descriptorSetCount = kMaxFramesInFlight;
		allocInfo.pSetLayouts = descriptorSetLayouts.Buffer();

		if (const VkResult result = vkAllocateDescriptorSets(logicalDevice.Get(), &allocInfo, m_sets.Buffer()); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to allocate descriptor sets! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		for (size_t i = 0; i < kMaxFramesInFlight; ++i)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniformBuffers[i].Get();
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_sets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;

			vkUpdateDescriptorSets(logicalDevice.Get(), 1, &descriptorWrite, 0, nullptr);
		}

		return true;
	}
}