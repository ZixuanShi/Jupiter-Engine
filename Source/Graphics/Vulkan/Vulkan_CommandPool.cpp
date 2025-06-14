// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan_CommandPool;

import jpt.Application;
import jpt.Renderer_Vulkan;

import jpt.Vulkan_PhysicalDevice;
import jpt.Vulkan_LogicalDevice;

namespace jpt::Vulkan
{
    bool CommandPool::Init()
    {
        const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
        const PhysicalDevice& physicalDevice = pVulkanRenderer->GetPhysicalDevice();
        const LogicalDevice& logicalDevice = pVulkanRenderer->GetLogicalDevice();

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = physicalDevice.GetGraphicsFamilyIndex();

        if (const VkResult result = vkCreateCommandPool(logicalDevice.GetHandle(), &poolInfo, nullptr, &m_commandPool); result != VK_SUCCESS)
        {
            JPT_ERROR("Failed to create command pool: %d", result);
            return false;
        }

        return true;
    }

    void CommandPool::Shutdown()
    {
        const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
        const LogicalDevice& logicalDevice = pVulkanRenderer->GetLogicalDevice();

        vkDestroyCommandPool(logicalDevice.GetHandle(), m_commandPool, nullptr);
        m_commandPool = VK_NULL_HANDLE;
    }
}