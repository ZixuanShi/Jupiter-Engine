// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.IndexBuffer;

import jpt.Vulkan.Buffer;

import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Utilities;

export namespace jpt::Vulkan
{
	class IndexBuffer
	{
	public:
		using IndexType = uint32;

	private:
		Buffer m_buffer;
		size_t m_count = 0;

	public:
		bool Init(const DynamicArray<uint32>& indices);

		void Shutdown();

	public:
		VkBuffer GetBuffer() const { return m_buffer.GetHandle(); }
		size_t GetCount() const { return m_count; }
	};

	bool IndexBuffer::Init(const DynamicArray<uint32>& indices)
	{
		// Staging buffer
		VkBufferCreateInfo stagingBufferInfo{};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.size = indices.Size();
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkMemoryPropertyFlags stagingMemoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		Buffer stagingBuffer;
		if (const VkResult result = stagingBuffer.Create(stagingBufferInfo, stagingMemoryProperties); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create staging buffer: %d", result);
			return false;
		}

		stagingBuffer.MapMemory(indices.ConstBuffer(), indices.Size());

		// Index buffer
		VkBufferCreateInfo indexBufferInfo{};
		indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferInfo.size = indices.Size();
		indexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		indexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkMemoryPropertyFlags memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		if (const VkResult result = m_buffer.Create(indexBufferInfo, memoryProperties); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create index buffer: %d", result);
			return false;
		}

		m_buffer.Copy(stagingBuffer.GetHandle(), indices.Size());
		stagingBuffer.Shutdown();

		m_count = indices.Count();
		return true;
	}

	void IndexBuffer::Shutdown()
	{
		m_buffer.Shutdown();
		m_count = 0;
	}
}