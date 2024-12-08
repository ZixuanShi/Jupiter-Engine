// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Utils;

import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.CommandPool;

import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	uint32 FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties, const PhysicalDevice& physicalDevice)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice.Get(), &memProperties);

		for (uint32 i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			if ((typeFilter & (1 << i)) &&  // Check if the memory type is suitable
				(memProperties.memoryTypes[i].propertyFlags & properties) == properties) // Check if the memory type has the required properties
			{
				return i;
			}
		}

		JPT_ASSERT(false, "Failed to find suitable memory type");
		return 0;
	}

	void CreateBuffer(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkDevice device = logicalDevice.Get();

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (const VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &buffer); result != VkResult::VK_SUCCESS)
		{
			JPT_ERROR("Failed to create vertex buffer! VkResult: %i", static_cast<uint32>(result));
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

		if (const VkResult result = vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory); result != VkResult::VK_SUCCESS)
		{
			JPT_ERROR("Failed to allocate vertex buffer memory! VkResult: %i", static_cast<uint32>(result));
		}

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

	void CopyBuffer(const LogicalDevice& logicalDevice, const CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool.Get();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(logicalDevice.Get(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0; // Optional
		copyRegion.dstOffset = 0; // Optional
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(logicalDevice.GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(logicalDevice.GetGraphicsQueue());

		vkFreeCommandBuffers(logicalDevice.Get(), commandPool.Get(), 1, &commandBuffer);
	}
}