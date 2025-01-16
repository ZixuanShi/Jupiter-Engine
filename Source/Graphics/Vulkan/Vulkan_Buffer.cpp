// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>
#include <memory>

module jpt.Vulkan.Buffer;

import jpt.Application;
import jpt.Renderer_Vulkan;

import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.Utils;

namespace jpt::Vulkan
{
	VkResult Buffer::Create(const VkBufferCreateInfo& createInfo, VkMemoryPropertyFlags properties)
	{
		const Renderer_Vulkan* pVulkanRenderer = GetApplication()->GetRenderer<Renderer_Vulkan>();
		const PhysicalDevice& physicalDevice = pVulkanRenderer->GetPhysicalDevice();
		const LogicalDevice& logicalDevice = pVulkanRenderer->GetLogicalDevice();

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

	void Buffer::Copy(VkBuffer srcBuffer, VkDeviceSize size)
	{
		const Renderer_Vulkan* pVulkanRenderer = GetApplication()->GetRenderer<Renderer_Vulkan>();
		const CommandPool& memoryTransferCommandPool = pVulkanRenderer->GetMemoryTransferCommandPool();

		VkCommandBuffer commandBuffer = BeginSingleTimeCommand(memoryTransferCommandPool);
		{
			VkBufferCopy copyRegion{};
			copyRegion.size = size;
			vkCmdCopyBuffer(commandBuffer, srcBuffer, m_buffer, 1, &copyRegion);
		}
		EndSingleTimeCommand(memoryTransferCommandPool, commandBuffer);
	}

	void Buffer::MapMemory(const void* pPtr, VkDeviceSize size)
	{
		const Renderer_Vulkan* pVulkanRenderer = GetApplication()->GetRenderer<Renderer_Vulkan>();
		const LogicalDevice& logicalDevice = pVulkanRenderer->GetLogicalDevice();

		void* pData = nullptr;
		vkMapMemory(logicalDevice.GetHandle(), m_bufferMemory, 0, size, 0, &pData);
		{
			memcpy(pData, pPtr, size);
		}
		vkUnmapMemory(logicalDevice.GetHandle(), m_bufferMemory);
	}

	void Buffer::Shutdown()
	{
		const Renderer_Vulkan* pVulkanRenderer = GetApplication()->GetRenderer<Renderer_Vulkan>();
		const LogicalDevice& logicalDevice = pVulkanRenderer->GetLogicalDevice();

		vkDestroyBuffer(logicalDevice.GetHandle(), m_buffer, nullptr);
		vkFreeMemory(logicalDevice.GetHandle(), m_bufferMemory, nullptr);
	}
}