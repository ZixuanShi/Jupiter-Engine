// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan_LogicalDevice;

import jpt.Application;
import jpt.Renderer_Vulkan;

import jpt.Vulkan_ValidationLayers;
import jpt.Vulkan_PhysicalDevice;
import jpt.Vulkan_Extensions;

namespace jpt::Vulkan
{
    bool LogicalDevice::Init()
    {
        const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
        const PhysicalDevice& physicalDevice = pVulkanRenderer->GetPhysicalDevice();

        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = physicalDevice.GetGraphicsFamilyIndex();
        queueCreateInfo.queueCount = 1;

        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        deviceFeatures.sampleRateShading = VK_TRUE;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32>(g_deviceExtensions.Count());
        createInfo.ppEnabledExtensionNames = g_deviceExtensions.ConstBuffer();

#if !IS_CONFIG_RELEASE
        createInfo.enabledLayerCount = static_cast<uint32>(g_validationLayers.Count());
        createInfo.ppEnabledLayerNames = g_validationLayers.ConstBuffer();
#else
        createInfo.enabledLayerCount = 0;
#endif

        if (vkCreateDevice(physicalDevice.GetHandle(), &createInfo, nullptr, &m_device) != VK_SUCCESS)
        {
            JPT_ERROR("Failed to create logical device!");
            return false;
        }

        vkGetDeviceQueue(m_device, physicalDevice.GetGraphicsFamilyIndex(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_device, physicalDevice.GetComputeFamilyIndex(), 0, &m_computeQueue);
        return true;
    }

    void LogicalDevice::Terminate()
    {
        vkDestroyDevice(m_device, nullptr);
    }

    void LogicalDevice::WaitIdle() const
    {
        vkDeviceWaitIdle(m_device);
    }

    LogicalDevice& LogicalDevice::Get()
    {
        Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
        return pVulkanRenderer->GetLogicalDevice();
    }

    VkDevice LogicalDevice::GetVkDevice()
    {
        return Get().GetHandle();
    }

    VkQueue LogicalDevice::GetGraphicsVkQueue()
    {
        return Get().GetGraphicsQueue();
    }
}