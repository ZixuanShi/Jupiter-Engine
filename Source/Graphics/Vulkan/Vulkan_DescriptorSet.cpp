// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan.DescriptorSet;

import jpt.Application;
import jpt.Renderer_Vulkan;
import jpt.Graphics.Constants;
import jpt.Texture.Sampler;

import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.DescriptorSetLayout;
import jpt.Vulkan.DescriptorPool;
import jpt.Vulkan.Texture.Sampler;

namespace jpt::Vulkan
{
	bool DescriptorSet::Init(const UniformBuffer& uniformBuffer, VkImageView textureImageView)
	{
		const Renderer_Vulkan* pVulkanRenderer = GetApplication()->GetRenderer<Renderer_Vulkan>();
		const LogicalDevice& logicalDevice = pVulkanRenderer->GetLogicalDevice();
		const DescriptorSetLayout& descriptorSetLayout = pVulkanRenderer->GetDescriptorSetLayout();
		const DescriptorPool& descriptorPool = pVulkanRenderer->GetDescriptorPool();

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

		TextureSampler* pTextureSampler = GetApplication()->GetRenderer()->GetTextureSampler();
		TextureSampler_Vulkan* pTextureSamplerVulkan = static_cast<TextureSampler_Vulkan*>(pTextureSampler);
		VkSampler textureSampler = pTextureSamplerVulkan->GetHandle();

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;

		VkWriteDescriptorSet descriptorWrites[2];

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].pNext = nullptr;
		descriptorWrites[0].dstSet = m_descriptorSet;
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;
		descriptorWrites[0].pImageInfo = nullptr;
		descriptorWrites[0].pTexelBufferView = nullptr;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].pNext = nullptr;
		descriptorWrites[1].dstSet = m_descriptorSet;
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pBufferInfo = nullptr;
		descriptorWrites[1].pImageInfo = &imageInfo;
		descriptorWrites[1].pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(logicalDevice.GetHandle(), JPT_ARRAY_COUNT(descriptorWrites), descriptorWrites, 0, nullptr);

		return true;
	}

	void DescriptorSet::Shutdown()
	{
		vkFreeDescriptorSets(LogicalDevice::Get().GetHandle(), m_descriptorPool, 1, &m_descriptorSet);
	}
}