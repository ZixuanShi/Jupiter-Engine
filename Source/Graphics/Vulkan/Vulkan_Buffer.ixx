// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>
#include <memory>

export module jpt.Vulkan.Buffer;

import jpt.Vulkan.Data;
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
		VkResult Create(const VkBufferCreateInfo& createInfo, const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkBuffer GetHandle() const { return m_buffer; }
		VkDeviceMemory GetBufferMemory() const { return m_bufferMemory; }
	};

	VkResult Buffer::Create(const VkBufferCreateInfo& createInfo, const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice)
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
		allocateInfo.memoryTypeIndex = physicalDevice.FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (const VkResult result = vkAllocateMemory(logicalDevice.GetHandle(), &allocateInfo, nullptr, &m_bufferMemory); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to allocate buffer memory: %d", result);
			return result;
		}

		vkBindBufferMemory(logicalDevice.GetHandle(), m_buffer, m_bufferMemory, 0);

		void* pData = nullptr;
		vkMapMemory(logicalDevice.GetHandle(), m_bufferMemory, 0, createInfo.size, 0, &pData);
		{
			memcpy(pData, vertices.ConstBuffer(), vertices.Size());
		}
		vkUnmapMemory(logicalDevice.GetHandle(), m_bufferMemory);

		return VK_SUCCESS;
	}

	void Buffer::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyBuffer(logicalDevice.GetHandle(), m_buffer, nullptr);
		vkFreeMemory(logicalDevice.GetHandle(), m_bufferMemory, nullptr);
	}
}