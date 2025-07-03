// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan_DescriptorPool;

import jpt.Application;

import jpt.Graphics_Constants;
import jpt.Vulkan_LogicalDevice;

import jpt.WindowManager;

namespace jpt::Vulkan
{
    bool DescriptorPool::Init()
    {
        // Create pool sizes for both descriptor types
        VkDescriptorPoolSize poolSizes[2];

        // Uniform buffer descriptors
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32>(kMaxFramesInFlight * WindowManager::kMaxWindows);

        // Combined image sampler descriptors
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32>(kMaxFramesInFlight * WindowManager::kMaxWindows);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 2;  // Changed from 1 to 2
        poolInfo.pPoolSizes = poolSizes;  // Point to array
        poolInfo.maxSets = static_cast<uint32>(kMaxFramesInFlight * WindowManager::kMaxWindows);
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        if (const VkResult result = vkCreateDescriptorPool(LogicalDevice::GetVkDevice(), &poolInfo, nullptr, &m_descriptorPool); result != VK_SUCCESS)
        {
            JPT_ERROR("Failed to create descriptor pool: %d", result);
            return false;
        }

        return true;
    }

    void DescriptorPool::Terminate()
    {
        vkDestroyDescriptorPool(LogicalDevice::GetVkDevice(), m_descriptorPool, nullptr);
    }
}