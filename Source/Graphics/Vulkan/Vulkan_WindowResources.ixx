// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module jpt.Vulkan.WindowResources;

import jpt.Window;

import jpt.Graphics.Constants;

import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.RenderPass;
import jpt.Vulkan.Pipeline;
import jpt.Vulkan.PipelineLayout;
import jpt.Vulkan.DescriptorSets;

import jpt.Vulkan.Constants;
import jpt.Vulkan.SwapChain;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.CommandBuffers;
import jpt.Vulkan.SynchronizationObjects;
import jpt.Vulkan.VertexBuffer;
import jpt.Vulkan.IndexBuffer;
import jpt.Vulkan.UniformBufferObject;
import jpt.Vulkan.UniformBuffer;

import jpt.DynamicArray;
import jpt.StaticArray;

import jpt.TypeDefs;
import jpt.Time.TypeDefs;
import jpt.StopWatch;
import jpt.Math;
import jpt.Matrix44;

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
		StaticArray<UniformBuffer, kMaxFramesInFlight> m_uniformBuffers;

		size_t m_currentFrame = 0;

		bool m_shouldRecreateSwapChain = false;

	public:
		bool CreateSwapChain(const PhysicalDevice& physicalDevice);
		bool CreateImageViews();
		bool CreateFramebuffers(const RenderPass& renderPass);
		bool CreateCommandPool(uint32 queueFamilyIndex);
		bool CreateCommandBuffers();
		bool CreateSynchronizationObjects();
		bool CreateUniformBuffers(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice);

		void RecreateSwapChain(const PhysicalDevice& physicalDevice, RenderPass& renderPass, Pipeline& graphicsPipeline, PipelineLayout& pipelineLayout);

		void Shutdown(VkInstance instance);

		void DrawFrame(RenderPass& renderPass, Pipeline& graphicsPipeline, const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer,
			const PipelineLayout& pipelineLayout, DescriptorSets& descriptorSets);

		bool CanDraw() const;

	public:
		Window* GetOwner() const { return m_pOwner; }
		void SetOwner(Window* pOwner) { m_pOwner = pOwner; }

		void SetLogicalDevice(LogicalDevice* pLogicalDevice) { m_pLogicalDevice = pLogicalDevice; }

		bool ShouldRecreateSwapChain() const { return m_shouldRecreateSwapChain; }

		VkSurfaceKHR GetSurface() const { return m_surface; }
		VkSurfaceKHR* GetSurfacePtr() { return &m_surface; }

		const SwapChain& GetSwapChain() const { return m_swapChain; }
		VkFormat GetImageFormat() const { return m_swapChain.GetImageFormat(); }

		const CommandPool& GetCommandPool() const { return m_commandPool; }

		const StaticArray<UniformBuffer, kMaxFramesInFlight>& GetUniformBuffers() const { return m_uniformBuffers; }

	private:
		void UpdateUniformBuffer(size_t currentFrame);
	};

	bool WindowResources::CreateSwapChain(const PhysicalDevice& physicalDevice)
	{
		return m_swapChain.Init(*m_pLogicalDevice, physicalDevice, m_surface);
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

	bool WindowResources::CreateUniformBuffers(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice)
	{
		bool success = true;
		for (UniformBuffer& uniformBuffer : m_uniformBuffers)
		{
			success &= uniformBuffer.Init(logicalDevice, physicalDevice);
		}
		return success;
	}

	void WindowResources::RecreateSwapChain(const PhysicalDevice& physicalDevice, RenderPass& renderPass, Pipeline& graphicsPipeline, PipelineLayout& pipelineLayout)
	{
		m_pLogicalDevice->WaitIdle();

		JPT_ASSERT(!m_pOwner->IsMinimized());

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
			graphicsPipeline.Init(*m_pLogicalDevice, pipelineLayout,renderPass);
		}

		m_swapChain.CreateFramebuffers(*m_pLogicalDevice, renderPass);

		m_shouldRecreateSwapChain = false;
	}

	void WindowResources::Shutdown(VkInstance instance)
	{
		m_pLogicalDevice->WaitIdle();

		// Uniform buffers
		for (UniformBuffer& uniformBuffer : m_uniformBuffers)
		{
			uniformBuffer.Shutdown(*m_pLogicalDevice);
		}

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

	void WindowResources::DrawFrame(RenderPass& renderPass, Pipeline& graphicsPipeline, const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer,
		const PipelineLayout& pipelineLayout, DescriptorSets& descriptorSets)
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

		UpdateUniformBuffer(m_currentFrame);

		vkResetFences(m_pLogicalDevice->Get(), 1, currentSyncObjects.GetInFlightFencePtr());

		m_commandBuffers.Reset(m_currentFrame);
		m_commandBuffers.Record(m_currentFrame, imageIndex, renderPass, m_swapChain, graphicsPipeline, vertexBuffer, indexBuffer, pipelineLayout, descriptorSets);

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

	bool WindowResources::CanDraw() const
	{
		if (m_pOwner->IsMinimized() ||
			m_shouldRecreateSwapChain)
		{
			return false;
		}

		return true;
	}

	void WindowResources::UpdateUniformBuffer(size_t currentFrame)
	{
		static StopWatch::Point startTime = StopWatch::Now();

		const StopWatch::Point currentTime = StopWatch::Now();
		const float time = static_cast<float>(StopWatch::GetSecondsBetween(startTime, currentTime));

		UniformBufferObject ubo{};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), m_swapChain.GetExtent().width / (float)m_swapChain.GetExtent().height, 0.1f, 10.0f);
		ubo.proj[1][1] *= -1;

		memcpy(m_uniformBuffers[currentFrame].GetMappedMemory(), &ubo, sizeof(ubo));
	}
}