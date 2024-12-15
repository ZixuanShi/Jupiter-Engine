// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.VertexBuffer;

import jpt.Vulkan.Buffer;

import jpt.Vulkan.Data;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;

export namespace jpt::Vulkan
{
	class VertexBuffer
	{
	private:
		Buffer m_buffer;

	public:
		bool Init(const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkBuffer GetBuffer() { return m_buffer.GetHandle(); }
	};

	bool VertexBuffer::Init(const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = vertices.Size();
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (const VkResult result = m_buffer.Create(bufferInfo, logicalDevice, physicalDevice); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create vertex buffer: %d", result);
			return false;
		}

		return true;
	}

	void VertexBuffer::Shutdown(const LogicalDevice& logicalDevice)
	{
		m_buffer.Shutdown(logicalDevice);
	}
}