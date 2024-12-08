// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <memory>

#include <vulkan/vulkan.h>

export module jpt.Vulkan.IndexBuffer;

import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.Data;
import jpt.Vulkan.Utils;

export namespace jpt::Vulkan
{
	class IndexBuffer
	{
	private:
		VkBuffer m_indexBuffer;
		VkDeviceMemory m_indexBufferMemory;

	public:
		bool Init(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice, const CommandPool& commandPool);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkBuffer Get() const { return m_indexBuffer; }
	};

	bool IndexBuffer::Init(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice, const CommandPool& commandPool)
	{
		VkDeviceSize bufferSize = indices.Size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* pData = nullptr;
		vkMapMemory(logicalDevice.Get(), stagingBufferMemory, 0, bufferSize, 0, &pData);
		{
			memcpy(pData, indices.ConstBuffer(), (size_t)bufferSize);
		}
		vkUnmapMemory(logicalDevice.Get(), stagingBufferMemory);

		CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);

		CopyBuffer(logicalDevice, commandPool, stagingBuffer, m_indexBuffer, bufferSize);

		vkDestroyBuffer(logicalDevice.Get(), stagingBuffer, nullptr);
		vkFreeMemory(logicalDevice.Get(), stagingBufferMemory, nullptr);

		return true;
	}

	void IndexBuffer::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyBuffer(logicalDevice.Get(), m_indexBuffer, nullptr);
		vkFreeMemory(logicalDevice.Get(), m_indexBufferMemory, nullptr);
	}
}