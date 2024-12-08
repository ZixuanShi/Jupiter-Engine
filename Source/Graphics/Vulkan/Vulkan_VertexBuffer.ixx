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

import jpt.Vulkan.Data;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.Utils;

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
	};

	bool VertexBuffer::Init(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice, const CommandPool& commandPool)
	{
		VkDeviceSize bufferSize = vertices.Size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		VkDevice device = logicalDevice.Get();
		void* pData = nullptr;
		vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &pData);
		{
			memcpy(pData, vertices.ConstBuffer(), (size_t)bufferSize);
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
}