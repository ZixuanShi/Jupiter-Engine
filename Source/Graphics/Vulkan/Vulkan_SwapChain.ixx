// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

export module jpt.Vulkan.SwapChain;

import jpt.DynamicArray;

export namespace jpt
{
	class Window;

	export namespace Vulkan
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
			bool Init(Window* pWindow, VkSurfaceKHR surface);
			bool CreateImageViews();
			bool CreateFramebuffers(VkImageView depthImageView);

			void Shutdown();

		public:
			VkSwapchainKHR GetHandle() const { return m_swapChain; }
			const DynamicArray<VkImageView>& GetImageViews() const { return m_imageViews; }
			const DynamicArray<VkFramebuffer>& GetFramebuffers() const { return m_framebuffers; }
			VkFormat GetImageFormat() const { return m_imageFormat; }
			const VkExtent2D& GetExtent() const { return m_extent; }
		};
	}
}