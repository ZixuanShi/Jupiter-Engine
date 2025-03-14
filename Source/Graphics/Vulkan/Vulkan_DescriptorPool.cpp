// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan.DescriptorPool;

import jpt.Application;

import jpt.Graphics.Constants;
import jpt.Vulkan.LogicalDevice;

import jpt.Window.Manager;

namespace jpt::Vulkan
{
    bool DescriptorPool::Init()
    {
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = static_cast<uint32>(kMaxFramesInFlight);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = static_cast<uint32>(kMaxFramesInFlight * WindowManager::kMaxWindows);
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        if (const VkResult result = vkCreateDescriptorPool(LogicalDevice::GetVkDevice(), &poolInfo, nullptr, &m_descriptorPool); result != VK_SUCCESS)
        {
            JPT_ERROR("Failed to create descriptor pool: %d", result);
            return false;
        }

        return true;
    }

    void DescriptorPool::Shutdown()
    {
        vkDestroyDescriptorPool(LogicalDevice::GetVkDevice(), m_descriptorPool, nullptr);
    }
}