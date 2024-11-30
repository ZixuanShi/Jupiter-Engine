// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.WindowResources;

import jpt.Window;

import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.RenderPass;
import jpt.Vulkan.Pipeline;
import jpt.Vulkan.PipelineLayout;

import jpt.Vulkan.Constants;
import jpt.Vulkan.SwapChain;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.CommandBuffers;
import jpt.Vulkan.SynchronizationObjects;

import jpt.DynamicArray;
import jpt.StaticArray;

import jpt.TypeDefs;
import jpt.Time.TypeDefs;

export namespace jpt::Vulkan
{
	/** Per-Window specific Vulkan resource. Each Window should have its own data */
	class WindowResources
	{
	private:
		Window* m_pOwner = nullptr;
		LogicalDevice* m_pLogicalDevice = nullptr;

	private:
		VkSurfaceKHR m_surface;

		SwapChain m_swapChain;

		CommandPool m_commandPool;
		CommandBuffers m_commandBuffers;

		StaticArray<SynchronizationObjects, kMaxFramesInFlight> m_syncObjects;

		size_t m_currentFrame = 0;

		bool m_shouldRecreateSwapChain = false;

	public:
		bool CreateSwapChain(const PhysicalDevice& physicalDevice, VkSurfaceKHR surface);
		bool CreateImageViews();
		bool CreateFramebuffers(const RenderPass& renderPass);
		bool CreateCommandPool(uint32 queueFamilyIndex);
		bool CreateCommandBuffers();
		bool CreateSynchronizationObjects();

		void RecreateSwapChain(const PhysicalDevice& physicalDevice, RenderPass& renderPass, Pipeline& graphicsPipeline, PipelineLayout& pipelineLayout);

		void Shutdown(VkInstance instance);

		void DrawFrame(RenderPass& renderPass, Pipeline& graphicsPipeline);

	public:
		Window* GetOwner() const { return m_pOwner; }
		void SetOwner(Window* pOwner) { m_pOwner = pOwner; }

		void SetLogicalDevice(LogicalDevice* pLogicalDevice) { m_pLogicalDevice = pLogicalDevice; }

		bool ShouldRecreateSwapChain() const { return m_shouldRecreateSwapChain; }

		VkSurfaceKHR GetSurface() const { return m_surface; }
		VkSurfaceKHR* GetSurfacePtr() { return &m_surface; }

		const SwapChain& GetSwapChain() const { return m_swapChain; }
		VkFormat GetImageFormat() const { return m_swapChain.GetImageFormat(); }
	};

	bool WindowResources::CreateSwapChain(const PhysicalDevice& physicalDevice, VkSurfaceKHR surface)
	{
		return m_swapChain.Init(*m_pLogicalDevice, physicalDevice, surface);
	}

	bool WindowResources::CreateImageViews()
	{
		return m_swapChain.CreateImageViews(*m_pLogicalDevice);
	}

	bool WindowResources::CreateFramebuffers(const RenderPass& renderPass)
	{
		return m_swapChain.CreateFramebuffers(*m_pLogicalDevice, renderPass);
	}

	bool WindowResources::CreateCommandPool(uint32 queueFamilyIndex)
	{
		return m_commandPool.Init(*m_pLogicalDevice, queueFamilyIndex);
	}

	bool WindowResources::CreateCommandBuffers()
	{
		return m_commandBuffers.Init(*m_pLogicalDevice, m_commandPool);
	}

	bool WindowResources::CreateSynchronizationObjects()
	{
		bool success = true;
		for (SynchronizationObjects& syncObjects : m_syncObjects)
		{
			success &= syncObjects.Init(*m_pLogicalDevice);
		}
		return success;
	}

	void WindowResources::RecreateSwapChain(const PhysicalDevice& physicalDevice, RenderPass& renderPass, Pipeline& graphicsPipeline, PipelineLayout& pipelineLayout)
	{
		m_pLogicalDevice->WaitIdle();

		VkFormat previousFormat = m_swapChain.GetImageFormat();

		m_swapChain.Shutdown(*m_pLogicalDevice);
		m_swapChain.Init(*m_pLogicalDevice, physicalDevice, m_surface);

		m_swapChain.CreateImageViews(*m_pLogicalDevice);

		if (m_swapChain.GetImageFormat() != previousFormat)
		{
			renderPass.Shutdown(*m_pLogicalDevice);
			renderPass.Init(*m_pLogicalDevice, m_swapChain.GetImageFormat());

			graphicsPipeline.Shutdown(*m_pLogicalDevice);
			pipelineLayout.Shutdown(*m_pLogicalDevice);
			graphicsPipeline.Init(*m_pLogicalDevice, m_swapChain, pipelineLayout,renderPass);
		}

		m_swapChain.CreateFramebuffers(*m_pLogicalDevice, renderPass);

		m_shouldRecreateSwapChain = false;
	}

	void WindowResources::Shutdown(VkInstance instance)
	{
		m_pLogicalDevice->WaitIdle();

		// Synchronization objects
		for (SynchronizationObjects& syncObjects : m_syncObjects)
		{
			syncObjects.Shutdown(*m_pLogicalDevice);
		}

		// Command buffers and pool
		m_commandBuffers.Shutdown(*m_pLogicalDevice, m_commandPool);
		m_commandPool.Shutdown(*m_pLogicalDevice);

		// Swap chain resources
		m_swapChain.Shutdown(*m_pLogicalDevice);

		// Surface
		vkDestroySurfaceKHR(instance, m_surface, nullptr);

		m_pLogicalDevice = nullptr;
	}

	void WindowResources::DrawFrame(RenderPass& renderPass, Pipeline& graphicsPipeline)
	{
		SynchronizationObjects& currentSyncObjects = m_syncObjects[m_currentFrame];

		vkWaitForFences(m_pLogicalDevice->Get(), 1, currentSyncObjects.GetInFlightFencePtr(), VK_TRUE, UINT64_MAX);

		uint32 imageIndex = 0;
		const VkResult resultAcquireNextImage = vkAcquireNextImageKHR(m_pLogicalDevice->Get(), m_swapChain.Get(), UINT64_MAX, currentSyncObjects.GetImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);
		if (resultAcquireNextImage == VK_ERROR_OUT_OF_DATE_KHR)
		{
			m_shouldRecreateSwapChain = true;
			return;
		}
		JPT_ASSERT(resultAcquireNextImage == VK_SUCCESS || resultAcquireNextImage == VK_SUBOPTIMAL_KHR, "failed to acquire swap chain image %u", static_cast<uint32>(resultAcquireNextImage));

		vkResetFences(m_pLogicalDevice->Get(), 1, currentSyncObjects.GetInFlightFencePtr());

		m_commandBuffers.Reset(m_currentFrame);
		m_commandBuffers.Record(m_currentFrame, imageIndex, renderPass, m_swapChain, graphicsPipeline);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { currentSyncObjects.GetImageAvailableSemaphore() };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = m_commandBuffers.GetBufferPtr(m_currentFrame);

		VkSemaphore signalSemaphores[] = { currentSyncObjects.GetRenderFinishedSemaphore() };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (const VkResult result = vkQueueSubmit(m_pLogicalDevice->GetGraphicsQueue(), 1, &submitInfo, currentSyncObjects.GetInFlightFence()); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to submit draw command buffer! VkResult: %i", static_cast<uint32>(result));
			return;
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_swapChain.Get() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		const VkResult resultQueuePresent = vkQueuePresentKHR(m_pLogicalDevice->GetPresentQueue(), &presentInfo);
		if (resultQueuePresent == VK_ERROR_OUT_OF_DATE_KHR || resultQueuePresent == VK_SUBOPTIMAL_KHR)
		{
			m_shouldRecreateSwapChain = true;
		}
		else if (resultQueuePresent != VK_SUCCESS)
		{
			JPT_ERROR("Failed to present swap chain image! VkResult: %i", static_cast<uint32>(resultQueuePresent));
		}

		m_currentFrame = (m_currentFrame + 1) % kMaxFramesInFlight;
	}
}