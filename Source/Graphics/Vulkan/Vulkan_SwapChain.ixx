// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.SwapChain;

import jpt.Vulkan.SwapChainSupportDetails;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.RenderPass;

import jpt.DynamicArray;

export namespace jpt::Vulkan
{
	class SwapChain
	{
	private:
		DynamicArray<VkImage>       m_images;
		DynamicArray<VkImageView>   m_imageViews;
		DynamicArray<VkFramebuffer> m_framebuffers;

		VkSwapchainKHR m_swapChain;

		VkExtent2D m_extent;
		VkFormat m_imageFormat;

	public:
		bool Init(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice, VkSurfaceKHR surface);
		bool CreateImageViews(const LogicalDevice& logicalDevice);
		bool CreateFramebuffers(const LogicalDevice& logicalDevice, const RenderPass& renderPass);

		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		const DynamicArray<VkImage>&       GetImages()       const { return m_images;       }
		const DynamicArray<VkImageView>&   GetImageViews()   const { return m_imageViews;   }
		const DynamicArray<VkFramebuffer>& GetFramebuffers() const { return m_framebuffers; }
		VkSwapchainKHR Get() const { return m_swapChain; }
		VkFormat GetImageFormat() const { return m_imageFormat; }
		VkExtent2D GetExtent() const { return m_extent; }
	};

	bool SwapChain::Init(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice, VkSurfaceKHR surface)
	{
		SwapChainSupportDetails swapChainSupport(physicalDevice.Get(), surface);
		JPT_ASSERT(swapChainSupport.IsValid());

		const VkSurfaceFormatKHR surfaceFormat = swapChainSupport.ChooseSwapSurfaceFormat();
		const VkPresentModeKHR presentMode = swapChainSupport.ChooseSwapPresentMode();
		const VkExtent2D extent = swapChainSupport.ChooseSwapExtent();

		uint32 imageCount = swapChainSupport.GetImageCount();

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		const QueueFamilyIndices& queueFaimilyIndices = physicalDevice.GetQueueFamilyIndices();
		uint32 indices[] = { queueFaimilyIndices.graphicsFamily.Value(), queueFaimilyIndices.presentFamily.Value() };

		if (queueFaimilyIndices.graphicsFamily != queueFaimilyIndices.presentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = indices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapChainSupport.GetTransform();
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (const VkResult result = vkCreateSwapchainKHR(logicalDevice.Get(), &createInfo, nullptr, &m_swapChain); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create swap chain! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		vkGetSwapchainImagesKHR(logicalDevice.Get(), m_swapChain, &imageCount, nullptr);
		m_images.Resize(imageCount);
		vkGetSwapchainImagesKHR(logicalDevice.Get(), m_swapChain, &imageCount, m_images.Buffer());

		m_imageFormat = surfaceFormat.format;
		m_extent = extent;

		return true;
	}

	void SwapChain::Shutdown(const LogicalDevice& logicalDevice)
	{
		// Wait for device to finish operations
		logicalDevice.WaitIdle();

		// Destroy framebuffers
		for (VkFramebuffer framebuffer : m_framebuffers)
		{
			vkDestroyFramebuffer(logicalDevice.Get(), framebuffer, nullptr);
		}
		m_framebuffers.Clear();

		// Destroy image views
		for (VkImageView imageView : m_imageViews)
		{
			vkDestroyImageView(logicalDevice.Get(), imageView, nullptr);
		}
		m_imageViews.Clear();

		// Destroy render pass and swap chain
		vkDestroySwapchainKHR(logicalDevice.Get(), m_swapChain, nullptr);
	}

	bool SwapChain::CreateImageViews(const LogicalDevice& logicalDevice)
	{
		m_imageViews.Resize(m_images.Count());

		for (uint32 i = 0; i < m_images.Count(); ++i)
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

			if (const VkResult result = vkCreateImageView(logicalDevice.Get(), &createInfo, nullptr, &m_imageViews[i]); result != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create image views! VkResult: %i", static_cast<uint32>(result));
				return false;
			}
		}

		return true;
	}

	bool SwapChain::CreateFramebuffers(const LogicalDevice& logicalDevice, const RenderPass& renderPass)
	{
		m_framebuffers.Resize(m_imageViews.Count());

		for (size_t i = 0; i < m_imageViews.Count(); ++i)
		{
			VkImageView attachments[] = { m_imageViews[i] };

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass.Get();
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = m_extent.width;
			framebufferInfo.height = m_extent.height;
			framebufferInfo.layers = 1;

			if (const VkResult result = vkCreateFramebuffer(logicalDevice.Get(), &framebufferInfo, nullptr, &m_framebuffers[i]); result != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create framebuffer! VkResult: %i", static_cast<uint32>(result));
				return false;
			}
		}

		return true;
	}
}