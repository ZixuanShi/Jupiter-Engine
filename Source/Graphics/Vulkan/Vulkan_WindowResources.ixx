// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.WindowResources;

import jpt.Vulkan.Constants;
import jpt.Vulkan.SwapChain;

import jpt.DynamicArray;
import jpt.StaticArray;

export namespace jpt::Vulkan
{
	/** Per-Window specific Vulkan resource. Each Window should have its own data */
	class WindowResources
	{
	private:
		// Surface
		VkSurfaceKHR m_surface;

		SwapChain m_swapChain;

		// Command buffers
		VkCommandPool m_commandPool;
		StaticArray<VkCommandBuffer, kMaxFramesInFlight> m_commandBuffers;

		// Synchronization objects
		StaticArray<VkSemaphore, kMaxFramesInFlight> m_imageAvailableSemaphores;
		StaticArray<VkSemaphore, kMaxFramesInFlight> m_renderFinishedSemaphores;
		StaticArray<VkFence, kMaxFramesInFlight> m_inFlightFences;

		// Track frame index
		size_t m_currentFrame = 0;

	public:
	};
}