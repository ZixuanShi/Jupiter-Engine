// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.SyncObjects;

import jpt.Vulkan.LogicalDevice;

export namespace jpt::Vulkan
{
	/** Synchronization objects that are used to coordinate operations between the CPU and the GPU */
	class SyncObjects
	{
	private:
		VkSemaphore m_imageAvailableSemaphore = VK_NULL_HANDLE;	/**< Signaled when the presentation engine is finished using an image */
		VkSemaphore m_renderFinishedSemaphore = VK_NULL_HANDLE; /**< Signaled when the rendering operation is finished */
		VkFence m_inFlightFence = VK_NULL_HANDLE; /**< Signaled when a command buffer finishes execution */

	public:
		bool Init(const LogicalDevice& logicalDevice);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkSemaphore GetImageAvailableSemaphore() const { return m_imageAvailableSemaphore; }
		VkSemaphore GetRenderFinishedSemaphore() const { return m_renderFinishedSemaphore; }
		VkFence GetInFlightFence() const { return m_inFlightFence; }
		VkFence* GetInFlightFencePtr() { return &m_inFlightFence; }
	};

	bool SyncObjects::Init(const LogicalDevice& logicalDevice)
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateSemaphore(logicalDevice.GetHandle(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(logicalDevice.GetHandle(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS ||
			vkCreateFence(logicalDevice.GetHandle(), &fenceInfo, nullptr, &m_inFlightFence) != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create synchronization objects");
			return false;
		}

		return true;
	}

	void SyncObjects::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroySemaphore(logicalDevice.GetHandle(), m_imageAvailableSemaphore, nullptr);
		vkDestroySemaphore(logicalDevice.GetHandle(), m_renderFinishedSemaphore, nullptr);
		vkDestroyFence(logicalDevice.GetHandle(), m_inFlightFence, nullptr);
	}
}