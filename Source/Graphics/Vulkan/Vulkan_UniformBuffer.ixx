// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>
#include <glm/mat4x4.hpp>

export module jpt.Vulkan.UniformBuffer;

import jpt.Vulkan.Buffer;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;

import jpt.Matrix44;

export namespace jpt::Vulkan
{
	struct alignas(16) UniformBufferObject
	{
	public:
		//Matrix44f model;
		//Matrix44f view;
		//Matrix44f proj;

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	/** Holds constant across all shaders. Think of it like a global configuration that all our shaders can read from */
	class UniformBuffer
	{
	private:
		Buffer m_buffer;
		void* m_mappedMemory = nullptr;

	public:
		bool Init(const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkBuffer GetHandle() const { return m_buffer.GetHandle(); }
		void* GetMappedMemory() const { return m_mappedMemory; }
	};

	bool UniformBuffer::Init(const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice)
	{
		const VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		VkBufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = bufferSize;
		createInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkMemoryPropertyFlags memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		if (const VkResult result = m_buffer.Create(createInfo, memoryProperties, logicalDevice, physicalDevice); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create uniform buffer: %d", result);
			return false;
		}

		vkMapMemory(logicalDevice.GetHandle(), m_buffer.GetMemory(), 0, bufferSize, 0, &m_mappedMemory);
		return true;
	}

	void UniformBuffer::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkUnmapMemory(logicalDevice.GetHandle(), m_buffer.GetMemory());
		m_buffer.Shutdown(logicalDevice);
	}
}