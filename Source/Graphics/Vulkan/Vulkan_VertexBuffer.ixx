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

import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	class VertexBuffer
	{
	private:
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;

	public:
		bool Init(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkBuffer Get() const { return m_vertexBuffer; }

	private:
		uint32 FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties, const PhysicalDevice& physicalDevice);
	};

	bool VertexBuffer::Init(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice)
	{
		VkDevice device = logicalDevice.Get();

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = vertices.Size();
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (const VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &m_vertexBuffer); result != VkResult::VK_SUCCESS)
		{
			JPT_ERROR("Failed to create vertex buffer! VkResult: %i", static_cast<uint32>(result));
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, m_vertexBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, physicalDevice);

		if (const VkResult result = vkAllocateMemory(device, &allocInfo, nullptr, &m_vertexBufferMemory); result != VkResult::VK_SUCCESS)
		{
			JPT_ERROR("Failed to allocate vertex buffer memory! VkResult: %i", static_cast<uint32>(result));
		}

		vkBindBufferMemory(device, m_vertexBuffer, m_vertexBufferMemory, 0);

		void* data = nullptr;
		vkMapMemory(device, m_vertexBufferMemory, 0, bufferInfo.size, 0, &data);
		memcpy(data, vertices.ConstBuffer(), (size_t)bufferInfo.size);
		vkUnmapMemory(device, m_vertexBufferMemory);

		return true;
	}

	void VertexBuffer::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyBuffer(logicalDevice.Get(), m_vertexBuffer, nullptr);
		vkFreeMemory(logicalDevice.Get(), m_vertexBufferMemory, nullptr);
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
}