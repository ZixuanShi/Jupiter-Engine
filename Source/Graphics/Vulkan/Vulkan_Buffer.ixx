// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>
#include <memory>

export module jpt.Vulkan.Buffer;

import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;

export namespace jpt::Vulkan
{
	class Buffer
	{
	private:
		VkBuffer m_buffer = VK_NULL_HANDLE;
		VkDeviceMemory m_bufferMemory = VK_NULL_HANDLE;

	public:
		VkResult Create(const VkBufferCreateInfo& createInfo, VkMemoryPropertyFlags properties, const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice);
		void Copy(VkBuffer srcBuffer, VkDeviceSize size, const LogicalDevice& logicalDevice, const CommandPool& memoryTransferCommandPool);
		void MapMemory(const LogicalDevice& logicalDevice, const void* pPtr, VkDeviceSize size);

		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkBuffer GetHandle() const { return m_buffer; }
		VkDeviceMemory GetBufferMemory() const { return m_bufferMemory; }
	};

	VkResult Buffer::Create(const VkBufferCreateInfo& createInfo, VkMemoryPropertyFlags properties, const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice)
	{
		if (const VkResult result = vkCreateBuffer(logicalDevice.GetHandle(), &createInfo, nullptr, &m_buffer); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create buffer: %d", result);
			return result;
		}

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(logicalDevice.GetHandle(), m_buffer, &memoryRequirements);

		VkMemoryAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memoryRequirements.size;
		allocateInfo.memoryTypeIndex = physicalDevice.FindMemoryType(memoryRequirements.memoryTypeBits, properties);

		if (const VkResult result = vkAllocateMemory(logicalDevice.GetHandle(), &allocateInfo, nullptr, &m_bufferMemory); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to allocate buffer memory: %d", result);
			return result;
		}

		vkBindBufferMemory(logicalDevice.GetHandle(), m_buffer, m_bufferMemory, 0);
		return VK_SUCCESS;
	}

	void Buffer::Copy(VkBuffer srcBuffer, VkDeviceSize size, const LogicalDevice& logicalDevice, const CommandPool& memoryTransferCommandPool)
	{
		VkCommandBufferAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandPool = memoryTransferCommandPool.GetHandle();
		allocateInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(logicalDevice.GetHandle(), &allocateInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);
		{
			VkBufferCopy copyRegion{};
			copyRegion.size = size;
			vkCmdCopyBuffer(commandBuffer, srcBuffer, m_buffer, 1, &copyRegion);
		}
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(logicalDevice.GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(logicalDevice.GetGraphicsQueue());

		vkFreeCommandBuffers(logicalDevice.GetHandle(), memoryTransferCommandPool.GetHandle(), 1, &commandBuffer);
	}

	void Buffer::MapMemory(const LogicalDevice& logicalDevice, const void* pPtr, VkDeviceSize size)
	{
		void* pData = nullptr;
		vkMapMemory(logicalDevice.GetHandle(), m_bufferMemory, 0, size, 0, &pData);
		{
			memcpy(pData, pPtr, size);
		}
		vkUnmapMemory(logicalDevice.GetHandle(), m_bufferMemory);
	}

	void Buffer::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyBuffer(logicalDevice.GetHandle(), m_buffer, nullptr);
		vkFreeMemory(logicalDevice.GetHandle(), m_bufferMemory, nullptr);
	}
}