
// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.SynchronizationObjects;

import jpt.Vulkan.Constants;
import jpt.Vulkan.LogicalDevice;

export namespace jpt::Vulkan
{
	/** Synchronization objects are used to coordinate operations between the host and the device
		- They are used to synchronize operations between the CPU and the GPU
		- They are used to synchronize operations between different queues
		- They are used to synchronize operations within the same queue */
	class SynchronizationObjects
	{
	private:
		VkSemaphore m_imageAvailableSemaphore;
		VkSemaphore m_renderFinishedSemaphore;
		VkFence m_inFlightFence;

	public:
		bool Init(const LogicalDevice& logicalDevice);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		const VkSemaphore GetImageAvailableSemaphore() const { return m_imageAvailableSemaphore; }
		const VkSemaphore GetRenderFinishedSemaphore() const { return m_renderFinishedSemaphore; }

		const VkFence* GetInFlightFencePtr() const { return &m_inFlightFence; }
		VkFence GetInFlightFence() const { return m_inFlightFence; }
	};

	bool SynchronizationObjects::Init(const LogicalDevice& logicalDevice)
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (const VkResult result = vkCreateSemaphore(logicalDevice.Get(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create image available semaphore! VkResult: %i", static_cast<uint32>(result));
			return false;
		}
		if (const VkResult result = vkCreateSemaphore(logicalDevice.Get(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create render finished semaphore! VkResult: %i", static_cast<uint32>(result));
			return false;
		}
		if (const VkResult result = vkCreateFence(logicalDevice.Get(), &fenceInfo, nullptr, &m_inFlightFence); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create in flight fence! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	void SynchronizationObjects::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroySemaphore(logicalDevice.Get(), m_imageAvailableSemaphore, nullptr);
		vkDestroySemaphore(logicalDevice.Get(), m_renderFinishedSemaphore, nullptr);
		vkDestroyFence(logicalDevice.Get(), m_inFlightFence, nullptr);
	}
}