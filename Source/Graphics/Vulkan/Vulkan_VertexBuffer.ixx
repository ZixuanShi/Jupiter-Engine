// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>
#include <memory>

export module jpt.Vulkan.VertexBuffer;

import jpt.Vulkan.Buffer;

import jpt.Vulkan.Data;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;

import jpt.Time.TypeDefs;

export namespace jpt::Vulkan
{
	class VertexBuffer
	{
	private:
		Buffer m_buffer;

	public:
		bool Init();
		void Shutdown();

	public:
		VkBuffer GetBuffer() { return m_buffer.GetHandle(); }
	};

	bool VertexBuffer::Init()
	{
		// Staging buffer
		VkBufferCreateInfo stagingBufferInfo{};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.size = g_vertices.Size();
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkMemoryPropertyFlags stagingMemoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		Buffer stagingBuffer;
		if (const VkResult result = stagingBuffer.Create(stagingBufferInfo, stagingMemoryProperties); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create staging buffer: %d", result);
			return false;
		}

		stagingBuffer.MapMemory(g_vertices.ConstBuffer(), g_vertices.Size());

		// Vertex buffer
		VkBufferCreateInfo vertexBufferInfo{};
		vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vertexBufferInfo.size = g_vertices.Size();
		vertexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		vertexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkMemoryPropertyFlags memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		if (const VkResult result = m_buffer.Create(vertexBufferInfo, memoryProperties); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create vertex buffer: %d", result);
			return false;
		}

		m_buffer.Copy(stagingBuffer.GetHandle(), g_vertices.Size());
		stagingBuffer.Shutdown();

		return true;
	}

	void VertexBuffer::Shutdown()
	{
		m_buffer.Shutdown();
	}
}