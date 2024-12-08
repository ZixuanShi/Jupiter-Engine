// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

#include <glm/mat4x4.hpp>

export module jpt.Vulkan.UniformBuffer;

import jpt.Graphics.Constants;

import jpt.Vulkan.UniformBufferObject;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.Utils;

import jpt.StaticArray;

export namespace jpt::Vulkan
{
	class UniformBuffer
	{
	private:
		VkBuffer m_uniformBuffer;
		VkDeviceMemory m_uniformBufferMemory;
		void* m_pMappedMemory;

	public:
		bool Init(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		void* GetMappedMemory() const { return m_pMappedMemory; }
	};

	bool UniformBuffer::Init(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice)
	{
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffer, m_uniformBufferMemory);
		vkMapMemory(logicalDevice.Get(), m_uniformBufferMemory, 0, bufferSize, 0, &m_pMappedMemory);

		return true;
	}

	void UniformBuffer::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkUnmapMemory(logicalDevice.Get(), m_uniformBufferMemory);
		vkDestroyBuffer(logicalDevice.Get(), m_uniformBuffer, nullptr);
		vkFreeMemory(logicalDevice.Get(), m_uniformBufferMemory, nullptr);
	}
}