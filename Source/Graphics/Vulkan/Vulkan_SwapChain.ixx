// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.SwapChain;

import jpt.Vulkan.SwapChain.SupportDetails;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;

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
		DynamicArray<VkImage>     m_images;
		DynamicArray<VkImageView> m_imageViews;

		VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
		VkFormat m_imageFormat = VK_FORMAT_UNDEFINED;
		VkExtent2D m_extent;

	public:
		bool Init(Window* pWindow, const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, VkSurfaceKHR surface);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkSwapchainKHR GetHandle() const { return m_swapChain; }

	private:
		bool CreateSwapChain(Window* pWindow, const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, VkSurfaceKHR surface);
		bool CreateImageViews(const LogicalDevice& logicalDevice);
	};

	bool SwapChain::Init(Window* pWindow, const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, VkSurfaceKHR surface)
	{
		bool success = true;

		success &= CreateSwapChain(pWindow, physicalDevice, logicalDevice, surface);
		success &= CreateImageViews(logicalDevice);

		return true;
	}

	void SwapChain::Shutdown(const LogicalDevice& logicalDevice)
	{
		for (VkImageView imageView : m_imageViews)
		{
			vkDestroyImageView(logicalDevice.GetHandle(), imageView, nullptr);
		}

		vkDestroySwapchainKHR(logicalDevice.GetHandle(), m_swapChain, nullptr);
	}

	bool SwapChain::CreateSwapChain(Window* pWindow, const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, VkSurfaceKHR surface)
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
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_imageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(logicalDevice.GetHandle(), &createInfo, nullptr, &m_imageViews[i]) != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create image views");
				return false;
			}
		}

		return true;
	}
}