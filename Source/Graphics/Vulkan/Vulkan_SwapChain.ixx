// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.SwapChain;

import jpt.Vulkan.Utils;

import jpt.Vulkan.SwapChain.SupportDetails;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.RenderPass;

import jpt.Window;
import jpt.DynamicArray;
import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	/** A queue of images that are waiting to be presented to the screen.
		Front buffer is the image that is currently being displayed.
		Back buffer is the image that is currently being rendered to. */
	class SwapChain
	{
	private:
		DynamicArray<VkImage>       m_images;
		DynamicArray<VkImageView>   m_imageViews;
		DynamicArray<VkFramebuffer> m_framebuffers;

		VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
		VkFormat m_imageFormat = VK_FORMAT_UNDEFINED;
		VkExtent2D m_extent;

	public:
		bool Init(Window* pWindow, const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, VkSurfaceKHR surface);
		bool CreateImageViews(const LogicalDevice& logicalDevice);
		bool CreateFramebuffers(const LogicalDevice& logicalDevice, const RenderPass& renderPass, VkImageView depthImageView);

		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkSwapchainKHR GetHandle() const { return m_swapChain; }
		const DynamicArray<VkImageView>& GetImageViews() const { return m_imageViews; }
		const DynamicArray<VkFramebuffer>& GetFramebuffers() const { return m_framebuffers; }
		VkFormat GetImageFormat() const { return m_imageFormat; }
		const VkExtent2D& GetExtent() const { return m_extent; }
	};

	void SwapChain::Shutdown(const LogicalDevice& logicalDevice)
	{
		for (VkFramebuffer framebuffer : m_framebuffers)
		{
			vkDestroyFramebuffer(logicalDevice.GetHandle(), framebuffer, nullptr);
		}

		for (VkImageView imageView : m_imageViews)
		{
			vkDestroyImageView(logicalDevice.GetHandle(), imageView, nullptr);
		}

		vkDestroySwapchainKHR(logicalDevice.GetHandle(), m_swapChain, nullptr);
	}

	bool SwapChain::Init(Window* pWindow, const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, VkSurfaceKHR surface)
	{
		const SwapChainSupportDetails supportDetails = physicalDevice.QuerySwapChainSupport(surface);

		const VkSurfaceFormatKHR surfaceFormat = supportDetails.ChooseSwapSurfaceFormat();
		const VkPresentModeKHR presentMode = supportDetails.ChooseSwapPresentMode();
		const VkExtent2D extent = supportDetails.ChooseSwapExtent(pWindow);

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

	bool SwapChain::CreateImageViews(const LogicalDevice& logicalDevice)
	{
		uint32 imageCount = 0;

		vkGetSwapchainImagesKHR(logicalDevice.GetHandle(), m_swapChain, &imageCount, nullptr);
		m_images.Resize(imageCount);
		vkGetSwapchainImagesKHR(logicalDevice.GetHandle(), m_swapChain, &imageCount, m_images.Buffer());

		m_imageViews.Resize(imageCount);
		for (uint32 i = 0; i < imageCount; ++i)
		{
			m_imageViews[i] = CreateImageView(logicalDevice, m_images[i], m_imageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
		}

		return true;
	}

	bool SwapChain::CreateFramebuffers(const LogicalDevice& logicalDevice, const RenderPass& renderPass, VkImageView depthImageView)
	{
		m_framebuffers.Resize(m_imageViews.Count());

		for (uint32 i = 0; i < m_imageViews.Count(); ++i)
		{
			VkImageView attachments[] = 
			{
				m_imageViews[i],
				depthImageView
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
}