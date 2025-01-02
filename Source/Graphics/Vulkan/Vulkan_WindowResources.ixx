// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module jpt.Vulkan.WindowResources;

import jpt.Window;

import jpt.Graphics.Constants;
import jpt.Vulkan.Utils;

import jpt.Vulkan.Constants;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.Data;
import jpt.Vulkan.SwapChain;
import jpt.Vulkan.SwapChain.SupportDetails;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.RenderPass;
import jpt.Vulkan.PipelineLayout;
import jpt.Vulkan.GraphicsPipeline;
import jpt.Vulkan.SyncObjects;
import jpt.Vulkan.VertexBuffer;
import jpt.Vulkan.IndexBuffer;
import jpt.Vulkan.UniformBuffer;
import jpt.Vulkan.DescriptorSetLayout;
import jpt.Vulkan.DescriptorPool;
import jpt.Vulkan.DescriptorSet;

import jpt.Matrix44;
import jpt.Math;
import jpt.Optional;
import jpt.StaticArray;
import jpt.Utilities;
import jpt.TypeDefs;
import jpt.StopWatch;

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
		StaticArray<UniformBuffer, kMaxFramesInFlight> m_uniformBuffers;
		StaticArray<DescriptorSet, kMaxFramesInFlight> m_descriptorSets;

		// Depth Buffer
		VkImage m_depthImage;
		VkDeviceMemory m_depthImageMemory;
		VkImageView m_depthImageView;

		uint32 m_currentFrame = 0;
		bool m_shouldRecreateSwapChain = false;

	public:
		bool Init(Window* pWindow, VkInstance instance, 
			const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, const RenderPass& renderPass,
		    const DescriptorSetLayout& descriptorSetLayout, const DescriptorPool& descriptorPool,
			VkImageView textureImageView, VkSampler textureSampler);

		void Shutdown(VkInstance instance, const LogicalDevice& logicalDevice);

		void DrawFrame(const LogicalDevice& logicalDevice, const RenderPass& renderPass, const PipelineLayout& pipelineLayout, const GraphicsPipeline& graphicsPipeline,
			VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer);

		void RecreateSwapChain(const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, const RenderPass& renderPass);

	public:
		bool ShouldRecreateSwapChain() const;
		Window* GetOwner() const { return m_pOwner; }
		bool CanDraw() const;

	private:
		Optional<uint32> AcquireNextImage(const LogicalDevice& logicalDevice);
		void Record(const RenderPass& renderPass, const PipelineLayout& pipelineLayout, const GraphicsPipeline& graphicsPipeline, 
			VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer, uint32 imageIndex);
		void Submit() const;
		void Present(uint32& imageIndex);

		void UpdateUniformBuffer();

		void CreateDepthResources(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice);
		void DestroyDepthResources(const LogicalDevice& logicalDevice);
	};

	bool WindowResources::Init(Window* pWindow, VkInstance instance, 
		const PhysicalDevice& physicalDevice, const LogicalDevice& logicalDevice, const RenderPass& renderPass,
		const DescriptorSetLayout& descriptorSetLayout, const DescriptorPool& descriptorPool,
		VkImageView textureImageView, VkSampler textureSampler)
	{
		// Surface
		m_pOwner = pWindow;
		pWindow->CreateSurface({ instance, &m_surface });

		// Present queue
		const uint32 presentFamilyIndex = physicalDevice.FindPresentFamilyIndex(m_surface);
		vkGetDeviceQueue(logicalDevice.GetHandle(), presentFamilyIndex, 0, &m_presentQueue);

		// Depth Buffer
		CreateDepthResources(logicalDevice, physicalDevice);

		// SwapChain
		m_swapChain.Init(m_pOwner, physicalDevice, logicalDevice, m_surface);
		m_swapChain.CreateImageViews(logicalDevice);
		m_swapChain.CreateFramebuffers(logicalDevice, renderPass, m_depthImageView);

		// Command pool & buffers
		m_commandPool.Init(logicalDevice, physicalDevice.GetGraphicsFamilyIndex());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_commandPool.GetHandle();
		allocInfo.commandBufferCount = static_cast<uint32>(m_commandBuffers.Count());
		if (const VkResult result = vkAllocateCommandBuffers(logicalDevice.GetHandle(), &allocInfo, m_commandBuffers.Buffer()); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to allocate command buffers: %d", result);
			return false;
		}

		// Sync objects
		for (SyncObjects& syncObjects : m_syncObjects)
		{
			if (!syncObjects.Init(logicalDevice))
			{
				return false;
			}
		}

		// Uniform Buffers
		for (UniformBuffer& uniformBuffer : m_uniformBuffers)
		{
			if (!uniformBuffer.Init(physicalDevice, logicalDevice))
			{
				return false;
			}
		}

		// Descriptor sets
		for (size_t i = 0; i < kMaxFramesInFlight; ++i)
		{
			if (!m_descriptorSets[i].Init(logicalDevice, 
				descriptorSetLayout, descriptorPool, 
				m_uniformBuffers[i], textureImageView, textureSampler))
			{
				return false;
			}
		}
	
		return true;
	}

	void WindowResources::Shutdown(VkInstance instance, const LogicalDevice& logicalDevice)
	{
		logicalDevice.WaitIdle();

		vkDestroyImageView(logicalDevice.GetHandle(), m_depthImageView, nullptr);
		vkDestroyImage(logicalDevice.GetHandle(), m_depthImage, nullptr);
		vkFreeMemory(logicalDevice.GetHandle(), m_depthImageMemory, nullptr);

		for (DescriptorSet& descriptorSet : m_descriptorSets)
		{
			descriptorSet.Shutdown(logicalDevice);
		}

		for (uint32 i = 0; i < kMaxFramesInFlight; ++i)
		{
			m_uniformBuffers[i].Shutdown(logicalDevice);
		}

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

	void WindowResources::DrawFrame(const LogicalDevice& logicalDevice, const RenderPass& renderPass, const PipelineLayout& pipelineLayout, const GraphicsPipeline& graphicsPipeline,
		VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer)
	{
		SyncObjects& syncObjects = m_syncObjects[m_currentFrame];

		// Wait for the previous frame to finish
		vkWaitForFences(logicalDevice.GetHandle(), 1, syncObjects.GetInFlightFencePtr(), VK_TRUE, UINT64_MAX);

		if (Optional<uint32> imageIndex = AcquireNextImage(logicalDevice))
		{
			UpdateUniformBuffer();

			vkResetFences(logicalDevice.GetHandle(), 1, syncObjects.GetInFlightFencePtr());

			Record(renderPass, pipelineLayout, graphicsPipeline, vertexBuffer, indexBuffer, imageIndex.Value());
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
		DestroyDepthResources(logicalDevice);

		m_swapChain.Init(m_pOwner, physicalDevice, logicalDevice, m_surface);
		m_swapChain.CreateImageViews(logicalDevice);
		CreateDepthResources(logicalDevice, physicalDevice);
		m_swapChain.CreateFramebuffers(logicalDevice, renderPass, m_depthImageView);

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

	void WindowResources::Record(const RenderPass& renderPass, const PipelineLayout& pipelineLayout, const GraphicsPipeline& graphicsPipeline,
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

		VkClearValue clearValues[2];
		clearValues[0].color = { 0.05f, 0.05f, 0.05f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = JPT_ARRAY_COUNT(clearValues);
		renderPassInfo.pClearValues = clearValues;

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

			PushConstantData pushConstantData = {};
			pushConstantData.value = 1.0f;
			vkCmdPushConstants(commandBuffer, pipelineLayout.GetHandle(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantData), &pushConstantData);

			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout.GetHandle(), 0, 1, m_descriptorSets[m_currentFrame].GetHandlePtr(), 0, nullptr);
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

	void WindowResources::UpdateUniformBuffer()
	{
		static StopWatch::Point startTime = StopWatch::Now();

		const StopWatch::Point currentTime = StopWatch::Now();
		const float time = static_cast<float>(StopWatch::GetSecondsBetween(startTime, currentTime));
		const float rotation = time * ToRadians(90.0f);

		UniformBufferObject ubo = {};
		//ubo.model = Matrix44f::FromRadians(0, rotation, rotation);
		//ubo.view = Matrix44f::LookAt(Vec3f(2.0f, 2.0f, 2.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f::Forward());
		//ubo.proj = Matrix44f::Perspective(ToRadians(45.0f), m_pOwner->GetAspectRatio(), 0.1f, 10.0f);

		// TODO: make model per object transform
		ubo.model = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		ubo.model = glm::rotate(ubo.model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		ubo.view  = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ubo.proj  = glm::perspective(glm::radians(45.0f), m_pOwner->GetAspectRatio(), 0.1f, 10.0f);
		
		ubo.proj[1][1] *= -1;

		UniformBuffer& uniformBuffer = m_uniformBuffers[m_currentFrame];
		memcpy(uniformBuffer.GetMappedMemory(), &ubo, sizeof(ubo));
	}

	void WindowResources::CreateDepthResources(const LogicalDevice& logicalDevice, const PhysicalDevice& physicalDevice)
	{
		Vec2i frameSize = m_pOwner->GetFrameSize();
		const VkFormat depthFormat = physicalDevice.FindDepthFormat();

		CreateImage(logicalDevice, physicalDevice,
			frameSize.x, frameSize.y,
			depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_depthImage, m_depthImageMemory);

		m_depthImageView = CreateImageView(logicalDevice, m_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	}

	void WindowResources::DestroyDepthResources(const LogicalDevice& logicalDevice)
	{
		vkDestroyImageView(logicalDevice.GetHandle(), m_depthImageView, nullptr);
		vkDestroyImage(logicalDevice.GetHandle(), m_depthImage, nullptr);
		vkFreeMemory(logicalDevice.GetHandle(), m_depthImageMemory, nullptr);
	}

	bool WindowResources::CanDraw() const
	{
		bool canDraw = true;

		canDraw &= !m_pOwner->IsMinimized();

		return canDraw;
	}
}