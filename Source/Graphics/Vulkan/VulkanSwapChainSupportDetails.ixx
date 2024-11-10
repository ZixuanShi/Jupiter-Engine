// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.SwapChainSupportDetails;

import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Constants;
import jpt.Math;

export namespace jpt::Vulkan
{
	class SwapChainSupportDetails
	{
	private:
		VkSurfaceCapabilitiesKHR m_capabilities;	    /**< Basic surface capabilities (min/max number of images in swap chain, min/max width and height of images) */
		DynamicArray<VkSurfaceFormatKHR> m_formats;	    /**< Surface formats (pixel format, color space) */
		DynamicArray<VkPresentModeKHR> m_presentModes;  /**< Available presentation modes */

	public:
		bool Init(VkPhysicalDevice device, VkSurfaceKHR surface);

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat();
		VkPresentModeKHR ChooseSwapPresentMode();
		VkExtent2D ChooseSwapExtent();

		uint32 GetImageCount() const;
		VkSurfaceTransformFlagBitsKHR GetTransform() const;
		bool IsValid() const;
	};

	bool SwapChainSupportDetails::Init(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		// capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &m_capabilities);

		// formats
		uint32 formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
		if (formatCount != 0)
		{
			m_formats.Resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, m_formats.Buffer());
		}

		// present modes
		uint32 presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
		if (presentModeCount != 0)
		{
			m_presentModes.Resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, m_presentModes.Buffer());
		}

		return IsValid();
	}

	VkSurfaceFormatKHR SwapChainSupportDetails::ChooseSwapSurfaceFormat()
	{
		for (const VkSurfaceFormatKHR& availableFormat : m_formats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return m_formats[0];
	}

	VkPresentModeKHR SwapChainSupportDetails::ChooseSwapPresentMode()
	{
		for (const VkPresentModeKHR& availablePresentMode : m_presentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D SwapChainSupportDetails::ChooseSwapExtent()
	{
		if (m_capabilities.currentExtent.width != Constants<uint32>::kMax)
		{
			return m_capabilities.currentExtent;
		}
		else
		{
			VkExtent2D actualExtent = { 800, 600 };

			actualExtent.width = Max(m_capabilities.minImageExtent.width, Min(m_capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = Max(m_capabilities.minImageExtent.height, Min(m_capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	uint32 SwapChainSupportDetails::GetImageCount() const
	{
		uint32 imageCount = m_capabilities.minImageCount + 1;
		if (m_capabilities.maxImageCount > 0 && imageCount > m_capabilities.maxImageCount)
		{
			imageCount = m_capabilities.maxImageCount;
		}

		return imageCount;
	}

	VkSurfaceTransformFlagBitsKHR SwapChainSupportDetails::GetTransform() const
	{
		return m_capabilities.currentTransform;
	}

	bool SwapChainSupportDetails::IsValid() const
	{
		return !m_formats.IsEmpty() && !m_presentModes.IsEmpty();
	}
}