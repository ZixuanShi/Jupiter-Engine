// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan_SwapChain;

import jpt.Application;
import jpt.Renderer_Vulkan;

import jpt.Vulkan_Utils;
import jpt.Vulkan_SwapChain_SupportDetails;
import jpt.Vulkan_PhysicalDevice;
import jpt.Vulkan_LogicalDevice;
import jpt.Vulkan_RenderPass;

import jpt.Window;
import jpt.TypeDefs;

namespace jpt::Vulkan
{
    bool SwapChain::Init(Window* pWindow, VkSurfaceKHR surface)
    {
        const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
        const PhysicalDevice& physicalDevice = pVulkanRenderer->GetPhysicalDevice();
        const LogicalDevice& logicalDevice = pVulkanRenderer->GetLogicalDevice();

        const SwapChainSupportDetails supportDetails = physicalDevice.GetSwapChainSupport(surface);

        const VkSurfaceFormatKHR surfaceFormat = supportDetails.GetSurfaceFormat();
        const VkPresentModeKHR presentMode = supportDetails.GetPresentMode();
        const VkExtent2D extent = supportDetails.GetExtent(pWindow);

        uint32 imageCount = supportDetails.capabilities.minImageCount + 1;
        if (supportDetails.capabilities.maxImageCount > 0 && imageCount > supportDetails.capabilities.maxImageCount)
        {
            imageCount = supportDetails.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const uint32 graphicsFamilyIndex = physicalDevice.GetGraphicsFamilyIndex();
        const uint32 presentFamilyIndex = physicalDevice.FindPresentFamilyIndex(surface);
        const uint32 queueFamilyIndices[] = { graphicsFamilyIndex, presentFamilyIndex };

        if (graphicsFamilyIndex != presentFamilyIndex)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = supportDetails.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(logicalDevice.GetHandle(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        {
            JPT_ERROR("Failed to create swap chain");
            return false;
        }

        m_imageFormat = surfaceFormat.format;
        m_extent = extent;

        return true;
    }

    bool SwapChain::CreateImageViews()
    {
        VkDevice logicalDevice = LogicalDevice::GetVkDevice();

        uint32 imageCount = 0;

        vkGetSwapchainImagesKHR(logicalDevice, m_swapChain, &imageCount, nullptr);
        m_images.Resize(imageCount);
        vkGetSwapchainImagesKHR(logicalDevice, m_swapChain, &imageCount, m_images.Buffer());

        m_imageViews.Resize(imageCount);
        for (uint32 i = 0; i < imageCount; ++i)
        {
            m_imageViews[i] = CreateImageView(m_images[i], m_imageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }

        return true;
    }

    bool SwapChain::CreateFramebuffers(VkImageView colorImageView, VkImageView depthImageView)
    {
        const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
        const LogicalDevice& logicalDevice = pVulkanRenderer->GetLogicalDevice();
        const RenderPass& renderPass = pVulkanRenderer->GetRenderPass();

        m_framebuffers.Resize(m_imageViews.Count());

        for (uint32 i = 0; i < m_imageViews.Count(); ++i)
        {
            VkImageView attachments[] =
            {
                colorImageView,
                depthImageView,
                m_imageViews[i],
            };

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass.GetHandle();
            framebufferInfo.attachmentCount = JPT_ARRAY_COUNT(attachments);
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = m_extent.width;
            framebufferInfo.height = m_extent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(logicalDevice.GetHandle(), &framebufferInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS)
            {
                JPT_ERROR("Failed to create framebuffer");
                return false;
            }
        }

        return true;
    }

    void SwapChain::Shutdown()
    {
        for (VkFramebuffer framebuffer : m_framebuffers)
        {
            vkDestroyFramebuffer(LogicalDevice::GetVkDevice(), framebuffer, nullptr);
        }

        for (VkImageView imageView : m_imageViews)
        {
            vkDestroyImageView(LogicalDevice::GetVkDevice(), imageView, nullptr);
        }

        vkDestroySwapchainKHR(LogicalDevice::GetVkDevice(), m_swapChain, nullptr);
    }
}