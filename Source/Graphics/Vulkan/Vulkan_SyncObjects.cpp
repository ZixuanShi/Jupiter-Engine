// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan.SyncObjects;

import jpt.Vulkan.LogicalDevice;

namespace jpt::Vulkan
{
    bool SyncObjects::Init()
    {
        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(LogicalDevice::GetVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(LogicalDevice::GetVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(LogicalDevice::GetVkDevice(), &fenceInfo, nullptr, &m_inFlightFence) != VK_SUCCESS)
        {
            JPT_ERROR("Failed to create synchronization objects");
            return false;
        }

        return true;
    }

    void SyncObjects::Shutdown()
    {
        vkDestroySemaphore(LogicalDevice::GetVkDevice(), m_imageAvailableSemaphore, nullptr);
        vkDestroySemaphore(LogicalDevice::GetVkDevice(), m_renderFinishedSemaphore, nullptr);
        vkDestroyFence(LogicalDevice::GetVkDevice(), m_inFlightFence, nullptr);
    }
}