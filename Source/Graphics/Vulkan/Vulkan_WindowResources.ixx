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

import jpt.Vulkan.Vertex;
import jpt.Vulkan.SwapChain;
import jpt.Vulkan.SwapChain.SupportDetails;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.RenderPass;
import jpt.Vulkan.GraphicsPipeline;
import jpt.Vulkan.SyncObjects;
import jpt.Vulkan.VertexBuffer;
import jpt.Vulkan.IndexBuffer;

import jpt.Optional;
import jpt.StaticArray;
import jpt.Utilities;
import jpt.TypeDefs;

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
		StaticArray<VkCommandBuffer, kMaxFramesInFlight> m_commandBuffers;
		StaticArray<SyncObjects, kMaxFramesInFlight> m_syncObjects;

		uint32 m_currentFrame = 0;
		bool m_shouldRecreateSwapChain = false;

	public:
		bool Init(Window* pWindow, VkInstance instance, 
			const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, const RenderPass& renderPass);

		void Shutdown(VkInstance instance, const LogicalDevice& logicalDevice);

		void DrawFrame(const LogicalDevice& logicalDevice, const RenderPass& renderPass, const GraphicsPipeline& graphicsPipeline, 
			VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer);

		void RecreateSwapChain(const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, const RenderPass& renderPass);

		bool ShouldRecreateSwapChain() const;

		Window* GetOwner() const { return m_pOwner; }
		bool CanDraw() const;

	private:
		Optional<uint32> AcquireNextImage(const LogicalDevice& logicalDevice);
		void Record(const RenderPass& renderPass, const GraphicsPipeline& graphicsPipeline, VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer, uint32 imageIndex);
		void Submit() const;
		void Present(uint32& imageIndex);
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

		// Command pool & buffers
		m_commandPool.Init(logicalDevice, physicalDevice.GetGraphicsFamilyIndex());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_commandPool.GetHandle();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32>(m_commandBuffers.Count());
		if (const VkResult result = vkAllocateCommandBuffers(logicalDevice.GetHandle(), &allocInfo, m_commandBuffers.Buffer()); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to allocate command buffers: %d", result);
			return false;
		}

		// Sync objects
		for (SyncObjects& syncObjects : m_syncObjects)
		{
			syncObjects.Init(logicalDevice);
		}

		return true;
	}

	void WindowResources::Shutdown(VkInstance instance, const LogicalDevice& logicalDevice)
	{
		logicalDevice.WaitIdle();

		for (SyncObjects& syncObjects : m_syncObjects)
		{
			syncObjects.Shutdown(logicalDevice);
		}

		for (VkCommandBuffer commandBuffer : m_commandBuffers)
		{
			vkFreeCommandBuffers(logicalDevice.GetHandle(), m_commandPool.GetHandle(), 1, &commandBuffer);
		}

		m_commandPool.Shutdown(logicalDevice);
		m_swapChain.Shutdown(logicalDevice);
		
		vkDestroySurfaceKHR(instance, m_surface, nullptr);
		m_surface = VK_NULL_HANDLE;
	}

	void WindowResources::DrawFrame(const LogicalDevice& logicalDevice, const RenderPass& renderPass, const GraphicsPipeline& graphicsPipeline,
		VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer)
	{
		SyncObjects& syncObjects = m_syncObjects[m_currentFrame];

		// Wait for the previous frame to finish
		vkWaitForFences(logicalDevice.GetHandle(), 1, syncObjects.GetInFlightFencePtr(), VK_TRUE, UINT64_MAX);
		vkResetFences(logicalDevice.GetHandle(), 1, syncObjects.GetInFlightFencePtr());

		if (Optional<uint32> imageIndex = AcquireNextImage(logicalDevice))
		{
			Record(renderPass, graphicsPipeline, vertexBuffer, indexBuffer, imageIndex.Value());
			Submit();
			Present(imageIndex.Value());

			m_currentFrame += 1;
			m_currentFrame %= kMaxFramesInFlight;
		}
	}

	void WindowResources::RecreateSwapChain(const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, const RenderPass& renderPass)
	{
		logicalDevice.WaitIdle();

		m_swapChain.Shutdown(logicalDevice);

		m_swapChain.Init(m_pOwner, physicalDevice, logicalDevice, m_surface);
		m_swapChain.CreateImageViews(logicalDevice);
		m_swapChain.CreateFramebuffers(logicalDevice, renderPass);

		m_shouldRecreateSwapChain = false;
	}

	bool WindowResources::ShouldRecreateSwapChain() const
	{
		return m_shouldRecreateSwapChain;
	}

	Optional<uint32> WindowResources::AcquireNextImage(const LogicalDevice& logicalDevice)
	{
		uint32 imageIndex = 0;
		const VkResult result = vkAcquireNextImageKHR(logicalDevice.GetHandle(), m_swapChain.GetHandle(), UINT64_MAX, m_syncObjects[m_currentFrame].GetImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			m_shouldRecreateSwapChain = true;
			return {};
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			JPT_ERROR("Failed to acquire swap chain image: %d", result);
			return {};
		}

		return imageIndex;
	}

	void WindowResources::Record(const RenderPass& renderPass, const GraphicsPipeline& graphicsPipeline,
		VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer, uint32 imageIndex)
	{
		const VkCommandBuffer& commandBuffer = m_commandBuffers[m_currentFrame];
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

		VkClearValue clearColor = { 0.05f, 0.05f, 0.05f, 1.0f };
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

			VkBuffer vertexBuffers[] = { vertexBuffer.GetBuffer() };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

			if constexpr (AreSameType<uint16, decltype(g_indices)::TData>)
			{
				vkCmdBindIndexBuffer(commandBuffer, indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
			}
			else if constexpr (AreSameType<uint32, decltype(g_indices)::TData>)
			{
				vkCmdBindIndexBuffer(commandBuffer, indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
			}
			else
			{
				JPT_ASSERT(false, "Index buffer type not supported");
			}

			vkCmdDrawIndexed(commandBuffer, static_cast<uint32>(g_indices.Count()), 1, 0, 0, 0);
		}
		vkCmdEndRenderPass(commandBuffer);

		if (const VkResult result = vkEndCommandBuffer(commandBuffer); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to record command buffer: %d", result);
		}
	}

	void WindowResources::Submit() const
	{
		const SyncObjects& syncObjects = m_syncObjects[m_currentFrame];
		const VkCommandBuffer& commandBuffer = m_commandBuffers[m_currentFrame];

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { syncObjects.GetImageAvailableSemaphore() };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkSemaphore signalSemaphores[] = { m_syncObjects[m_currentFrame].GetRenderFinishedSemaphore() };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (const VkResult result = vkQueueSubmit(m_presentQueue, 1, &submitInfo, m_syncObjects[m_currentFrame].GetInFlightFence()); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to submit draw command buffer: %d", result);
		}
	}

	void WindowResources::Present(uint32& imageIndex)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		VkSemaphore signalSemaphores[] = { m_syncObjects[m_currentFrame].GetRenderFinishedSemaphore() };
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_swapChain.GetHandle() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		const VkResult result = vkQueuePresentKHR(m_presentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			m_shouldRecreateSwapChain = true;
			return;
		}
	}

	bool WindowResources::CanDraw() const
	{
		bool canDraw = true;

		canDraw &= !m_pOwner->IsMinimized();

		return canDraw;
	}
}