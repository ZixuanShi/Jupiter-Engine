// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan.UniformBuffer;

import jpt.Vulkan.Buffer;
import jpt.Vulkan.LogicalDevice;

import jpt.Matrix44;
import jpt.Utilities;

namespace jpt::Vulkan
{
    bool UniformBuffer::Init()
    {
        const VkDeviceSize bufferSize = sizeof(Uniform_MVP);

        VkBufferCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.size = bufferSize;
        createInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkMemoryPropertyFlags memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        if (const VkResult result = m_buffer.Create(createInfo, memoryProperties); result != VK_SUCCESS)
        {
            JPT_ERROR("Failed to create uniform buffer: %d", result);
            return false;
        }

        vkMapMemory(LogicalDevice::GetVkDevice(), m_buffer.GetMemory(), 0, bufferSize, 0, &m_mappedMemory);
        return true;
    }

    void UniformBuffer::Shutdown()
    {
        vkUnmapMemory(LogicalDevice::GetVkDevice(), m_buffer.GetMemory());
        m_buffer.Shutdown();
    }

    void UniformBuffer::MapMemory(void* pSource, VkDeviceSize size)
    {
        MemCpy(m_mappedMemory, pSource, size);
    }
}