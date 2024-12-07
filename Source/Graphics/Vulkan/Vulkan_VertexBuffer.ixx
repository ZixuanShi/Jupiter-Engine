// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <memory>

#include <vulkan/vulkan.h>

export module jpt.Vulkan.VertexBuffer;

import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.PhysicalDevice;

import jpt.Vulkan.Vertex;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;

import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	class VertexBuffer
	{
	private:
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;

	public:
		bool Init(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice, const CommandPool& commandPool);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkBuffer Get() const { return m_vertexBuffer; }

	private:
		void CreateBuffer(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		uint32 FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties, const PhysicalDevice& physicalDevice);
		void CopyBuffer(const LogicalDevice& logicalDevice, const CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	};

	bool VertexBuffer::Init(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice, const CommandPool& commandPool)
	{
		VkDeviceSize bufferSize = vertices.Size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		VkDevice device = logicalDevice.Get();
		void* data = nullptr;
		vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
		{
			memcpy(data, vertices.ConstBuffer(), (size_t)bufferSize); 
		}
		vkUnmapMemory(device, stagingBufferMemory);

		CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);
		CopyBuffer(logicalDevice, commandPool, stagingBuffer, m_vertexBuffer, bufferSize);
		
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);

		return true;
	}

	void VertexBuffer::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyBuffer(logicalDevice.Get(), m_vertexBuffer, nullptr);
		vkFreeMemory(logicalDevice.Get(), m_vertexBufferMemory, nullptr);
	}

	void VertexBuffer::CreateBuffer(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
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

	uint32 VertexBuffer::FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties, const PhysicalDevice& physicalDevice)
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

	void VertexBuffer::CopyBuffer(const LogicalDevice& logicalDevice, const CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
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