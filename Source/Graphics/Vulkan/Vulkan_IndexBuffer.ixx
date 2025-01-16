// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.IndexBuffer;

import jpt.Vulkan.Buffer;

import jpt.Vulkan.Data;

export namespace jpt::Vulkan
{
	class IndexBuffer
	{
	private:
		Buffer m_buffer;

	public:
		bool Init();
		void Shutdown();

	public:
		VkBuffer GetBuffer() const { return m_buffer.GetHandle(); }
	};

	bool IndexBuffer::Init()
	{
		// Staging buffer
		VkBufferCreateInfo stagingBufferInfo{};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.size = g_indices.Size();
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkMemoryPropertyFlags stagingMemoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		Buffer stagingBuffer;
		if (const VkResult result = stagingBuffer.Create(stagingBufferInfo, stagingMemoryProperties); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create staging buffer: %d", result);
			return false;
		}

		stagingBuffer.MapMemory(g_indices.ConstBuffer(), g_indices.Size());

		// Index buffer
		VkBufferCreateInfo indexBufferInfo{};
		indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferInfo.size = g_indices.Size();
		indexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		indexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkMemoryPropertyFlags memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		if (const VkResult result = m_buffer.Create(indexBufferInfo, memoryProperties); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create index buffer: %d", result);
			return false;
		}

		m_buffer.Copy(stagingBuffer.GetHandle(), g_indices.Size());
		stagingBuffer.Shutdown();

		return true;
	}

	void IndexBuffer::Shutdown()
	{
		m_buffer.Shutdown();
	}
}