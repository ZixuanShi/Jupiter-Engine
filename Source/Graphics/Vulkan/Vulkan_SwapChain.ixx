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
		DynamicArray<VkImage> m_images;
		VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
		VkFormat m_imageFormat = VK_FORMAT_UNDEFINED;
		VkExtent2D m_extent;

	public:
		bool Init(Window* pWindow, const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, VkSurfaceKHR surface);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkSwapchainKHR GetHandle() const { return m_swapChain; }
	};

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

		vkGetSwapchainImagesKHR(logicalDevice.GetHandle(), m_swapChain, &imageCount, nullptr);
		m_images.Resize(imageCount);
		vkGetSwapchainImagesKHR(logicalDevice.GetHandle(), m_swapChain, &imageCount, m_images.Buffer());

		m_imageFormat = surfaceFormat.format;
		m_extent = extent;

		return true;
	}

	void SwapChain::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroySwapchainKHR(logicalDevice.GetHandle(), m_swapChain, nullptr);
	}
}