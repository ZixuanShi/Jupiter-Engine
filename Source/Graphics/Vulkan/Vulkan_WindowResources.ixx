// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.WindowResources;

import jpt.Window;

import jpt.Graphics.Constants;

import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;

import jpt.Vulkan.SwapChain;
import jpt.Vulkan.SwapChain.SupportDetails;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.CommandBuffers;
import jpt.Vulkan.RenderPass;
import jpt.Vulkan.GraphicsPipeline;
import jpt.Vulkan.SyncObjects;

export namespace jpt::Vulkan
{
	/** Per-Window specific Vulkan resource. Each Window should have its own data */
	class WindowResources
	{
	private:
		Window* m_pOwner = nullptr;

	private:
		VkSurfaceKHR m_surface = VK_NULL_HANDLE;
		VkQueue m_presentQueue = VK_NULL_HANDLE;
		SwapChain m_swapChain;
		CommandPool m_commandPool;
		CommandBuffer m_commandBuffers;
		SyncObjects m_syncObjects;

	public:
		bool Init(Window* pWindow, VkInstance instance, 
			const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, const RenderPass& renderPass);

		void Shutdown(VkInstance instance, const LogicalDevice& logicalDevice);

		void DrawFrame(const LogicalDevice& logicalDevice, const RenderPass& renderPass, const GraphicsPipeline& graphicsPipeline);

	public:
		Window* GetOwner() const { return m_pOwner; }

	private:
		void Record(const RenderPass& renderPass, const GraphicsPipeline& graphicsPipeline, uint32 imageIndex);
		void Submit();
		void Present(uint32 imageIndex);
	};

	bool WindowResources::Init(Window* pWindow, VkInstance instance, 
		const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, const RenderPass& renderPass)
	{
		// Surface
		m_pOwner = pWindow;
		pWindow->CreateSurface({ instance, &m_surface });

		// Present queue
		const uint32 presentFamilyIndex = physicalDevice.FindPresentFamilyIndex(m_surface);
		vkGetDeviceQueue(logicalDevice.GetHandle(), presentFamilyIndex, 0, &m_presentQueue);

		// SwapChain
		m_swapChain.Init(m_pOwner, physicalDevice, logicalDevice, m_surface);
		m_swapChain.CreateImageViews(logicalDevice);
		m_swapChain.CreateFramebuffers(logicalDevice, renderPass);

		// Command pool
		m_commandPool.Init(logicalDevice, physicalDevice.GetGraphicsFamilyIndex());
		m_commandBuffers.Init(logicalDevice, m_commandPool);

		// Sync objects
		m_syncObjects.Init(logicalDevice);

		return true;
	}

	void WindowResources::Shutdown(VkInstance instance, const LogicalDevice& logicalDevice)
	{
		logicalDevice.WaitIdle();

		m_syncObjects.Shutdown(logicalDevice);
		m_commandPool.Shutdown(logicalDevice);
		m_swapChain.Shutdown(logicalDevice);
		
		vkDestroySurfaceKHR(instance, m_surface, nullptr);
		m_surface = VK_NULL_HANDLE;
	}

	void WindowResources::DrawFrame(const LogicalDevice& logicalDevice, const RenderPass& renderPass, const GraphicsPipeline& graphicsPipeline)
	{
		// Wait for the previous frame to finish
		vkWaitForFences(logicalDevice.GetHandle(), 1, m_syncObjects.GetInFlightFencePtr(), VK_TRUE, UINT64_MAX);
		vkResetFences(logicalDevice.GetHandle(), 1, m_syncObjects.GetInFlightFencePtr());

		// Acquire an image from the swap chain
		uint32 imageIndex = 0;
		vkAcquireNextImageKHR(logicalDevice.GetHandle(), m_swapChain.GetHandle(), UINT64_MAX, m_syncObjects.GetImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);

		Record(renderPass, graphicsPipeline, imageIndex);
		Submit();
		Present(imageIndex);
	}

	void WindowResources::Record(const RenderPass& renderPass, const GraphicsPipeline& graphicsPipeline, uint32 imageIndex)
	{
		VkCommandBuffer commandBuffer = m_commandBuffers.GetHandle();
		vkResetCommandBuffer(commandBuffer, 0);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (const VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to begin recording command buffer: %d", result);
			return;
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass.GetHandle();
		renderPassInfo.framebuffer = m_swapChain.GetFramebuffers()[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_swapChain.GetExtent();

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		// Start recording commands
		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		{
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetHandle());

			VkViewport viewport = {};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(m_swapChain.GetExtent().width);
			viewport.height = static_cast<float>(m_swapChain.GetExtent().height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

			VkRect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent = m_swapChain.GetExtent();
			vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

			vkCmdDraw(commandBuffer, 3, 1, 0, 0);
		}
		vkCmdEndRenderPass(commandBuffer);

		if (const VkResult result = vkEndCommandBuffer(commandBuffer); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to record command buffer: %d", result);
		}
	}

	void WindowResources::Submit()
	{
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_syncObjects.GetImageAvailableSemaphore() };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = m_commandBuffers.GetHandlePtr();

		VkSemaphore signalSemaphores[] = { m_syncObjects.GetRenderFinishedSemaphore() };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (const VkResult result = vkQueueSubmit(m_presentQueue, 1, &submitInfo, m_syncObjects.GetInFlightFence()); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to submit draw command buffer: %d", result);
		}
	}

	void WindowResources::Present(uint32 imageIndex)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		VkSemaphore signalSemaphores[] = { m_syncObjects.GetRenderFinishedSemaphore() };
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_swapChain.GetHandle() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		if (const VkResult result = vkQueuePresentKHR(m_presentQueue, &presentInfo); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to present swap chain image: %d", result);
		}
	}
}