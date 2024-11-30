// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"
#include "Debugging/Assert.h"

#include <vulkan/vulkan.h>

export module jpt.Renderer_Vulkan;

import jpt.Renderer;

import jpt.Window;
import jpt.Window.Manager;

import jpt.Vulkan.Constants;
import jpt.Vulkan.ValidationLayers;
import jpt.Vulkan.DebugMessenger;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.WindowResources;
import jpt.Vulkan.SwapChain;
import jpt.Vulkan.RenderPass;
import jpt.Vulkan.PipelineLayout;
import jpt.Vulkan.Pipeline;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.Helpers;
import jpt.Vulkan.QueueFamilyIndices;
import jpt.Vulkan.SwapChainSupportDetails;

import jpt.TypeDefs;
import jpt.Time.TypeDefs;

import jpt.DynamicArray;
import jpt.StaticArray;
import jpt.HashMap;

import jpt.File.IO;
import jpt.File.Enums;
import jpt.File.Path;
import jpt.File.Path.Utils;

import jpt.Event.Window.Create;
import jpt.Event.Window.Resize;

using namespace jpt::Vulkan;

export namespace jpt
{
	class Renderer_Vulkan final : public Renderer
	{
		using Super = Renderer;

	private:
		VkInstance m_instance;

#if !IS_RELEASE
		DebugMessenger m_debugMessenger;
#endif

		VkSurfaceKHR m_surface;

		PhysicalDevice m_physicalDevice;
		LogicalDevice m_logicalDevice;

		SwapChain m_swapChain;

		RenderPass m_renderPass;

		PipelineLayout m_pipelineLayout;
		Pipeline m_graphicsPipeline;

		CommandPool m_commandPool;
		StaticArray<VkCommandBuffer, kMaxFramesInFlight> m_commandBuffers;

		StaticArray<VkSemaphore, kMaxFramesInFlight> m_imageAvailableSemaphores;
		StaticArray<VkSemaphore, kMaxFramesInFlight> m_renderFinishedSemaphores;
		StaticArray<VkFence, kMaxFramesInFlight> m_inFlightFences;

		size_t m_currentFrame = 0;

	public:
		virtual bool Init() override;
		virtual void Shutdown() override;

		virtual void DrawFrame() override;

		virtual void OnWindowCreate(const Event_Window_Create& eventWindowCreate) override;
		virtual void OnWindowResize(const Event_Window_Resize& eventWindowResize) override;

	private:
		// Initialization
		bool CreateInstance();
		bool CreateSurface(Window* pWindow);
		bool CreateCommandBuffers();
		bool CreateSyncObjects();

		// Vulkan helpers
		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32 imageIndex);
		void RecreateSwapChain();
	};

	bool Renderer_Vulkan::Init()
	{
		JPT_ENSURE(Super::Init());

		uint32 extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		JPT_INFO("Extensions count: %i", extensionCount);

		bool success = true;

		success &= CreateInstance();

#if !IS_RELEASE
		success &= m_debugMessenger.Init(m_instance);
#endif

		success &= CreateSurface(GetApplication()->GetMainWindow());	// Main window

		success &= m_physicalDevice.Init(m_instance, m_surface);
		success &= m_logicalDevice.Init(m_physicalDevice);

		success &= m_swapChain.Init(m_logicalDevice, m_physicalDevice, m_surface);
		success &= m_swapChain.CreateImageViews(m_logicalDevice);

		success &= m_renderPass.Init(m_logicalDevice, m_swapChain.GetImageFormat());

		success &= m_pipelineLayout.Init(m_logicalDevice);
		success &= m_graphicsPipeline.Init(m_logicalDevice, m_swapChain, m_pipelineLayout, m_renderPass);

		success &= m_swapChain.CreateFramebuffers(m_logicalDevice, m_renderPass);

		const uint32 queueFamilyIndex = m_physicalDevice.GetQueueFamilyIndices().graphicsFamily.Value();
		success &= m_commandPool.Init(m_logicalDevice, queueFamilyIndex);
		success &= CreateCommandBuffers();

		success &= CreateSyncObjects();

		if (success)
		{
			JPT_INFO("Vulkan renderer initialized successfully");
		}

		return success;
	}

	void Renderer_Vulkan::Shutdown()
	{
		m_logicalDevice.WaitIdle();

		// Synchronization objects
		for (size_t i = 0; i < kMaxFramesInFlight; ++i)
		{
			vkDestroySemaphore(m_logicalDevice.Get(), m_imageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(m_logicalDevice.Get(), m_renderFinishedSemaphores[i], nullptr);
			vkDestroyFence(m_logicalDevice.Get(), m_inFlightFences[i], nullptr);
		}

		// Command buffers and pool
		vkFreeCommandBuffers(m_logicalDevice.Get(), m_commandPool.Get(),
			static_cast<uint32>(m_commandBuffers.Count()),
			m_commandBuffers.Buffer());
		m_commandPool.Shutdown(m_logicalDevice);

		// Swap chain resources
		m_swapChain.Shutdown(m_logicalDevice);

		// Render Pass
		m_renderPass.Shutdown(m_logicalDevice);

		// Pipeline resources
		m_graphicsPipeline.Shutdown(m_logicalDevice);
		m_pipelineLayout.Shutdown(m_logicalDevice);

		// Device
		m_logicalDevice.Shutdown();

		// Debugger
#if !IS_RELEASE
		m_debugMessenger.Shutdown(m_instance);
#endif

		// Instance-level resources
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		vkDestroyInstance(m_instance, nullptr);

		Super::Shutdown();
	}

	void Renderer_Vulkan::DrawFrame()
	{
		Super::DrawFrame();

		/**	At a high level, rendering a frame in Vulkan consists of a common set of steps:
			- Wait for the previous frame to finish
			- Acquire an image from the swap chain
			- Begin render pass (clear screen)
			- Record a command buffer which draws the scene onto that image
			- Submit the recorded command buffer
			- End render pass
			- Present the swap chain image	*/

		vkWaitForFences(m_logicalDevice.Get(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

		uint32 imageIndex = 0;
		const VkResult resultAcquireNextImage = vkAcquireNextImageKHR(m_logicalDevice.Get(), m_swapChain.Get(), UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);
		if (resultAcquireNextImage == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return;
		}
		JPT_ASSERT(resultAcquireNextImage == VK_SUCCESS || resultAcquireNextImage == VK_SUBOPTIMAL_KHR, "failed to acquire swap chain image %u", static_cast<uint32>(resultAcquireNextImage));
		
		vkResetFences(m_logicalDevice.Get(), 1, &m_inFlightFences[m_currentFrame]);

		vkResetCommandBuffer(m_commandBuffers[m_currentFrame], 0);
		RecordCommandBuffer(m_commandBuffers[m_currentFrame], imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphores[m_currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffers[m_currentFrame];

		VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (const VkResult result = vkQueueSubmit(m_logicalDevice.GetGraphicsQueue(), 1, &submitInfo, m_inFlightFences[m_currentFrame]); result != VK_SUCCESS)
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

		const VkResult resultQueuePresent = vkQueuePresentKHR(m_logicalDevice.GetPresentQueue(), &presentInfo);
		if (resultQueuePresent == VK_ERROR_OUT_OF_DATE_KHR || resultQueuePresent == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapChain();
		}
		else if (resultQueuePresent != VK_SUCCESS)
		{
			JPT_ERROR("Failed to present swap chain image! VkResult: %i", static_cast<uint32>(resultQueuePresent));
		}

		m_currentFrame = (m_currentFrame + 1) % kMaxFramesInFlight;
	}

	void Renderer_Vulkan::OnWindowCreate([[maybe_unused]] const Event_Window_Create& eventWindowCreate)
	{
		// The window is initialized with WindowManager. Create vulkan's per-window specific rendering resources here

		// Surface

		// Swap chain

		// Command buffers

		// Framebuffers

	}

	void Renderer_Vulkan::OnWindowResize(const Event_Window_Resize& eventWindowResize)
	{
		if (eventWindowResize.GetWidth() == 0 || eventWindowResize.GetHeight() == 0)
		{
			return;
		}

		RecreateSwapChain();
	}

	void Renderer_Vulkan::RecreateSwapChain()
	{
		m_logicalDevice.WaitIdle();

		VkFormat previousFormat = m_swapChain.GetImageFormat();

		m_swapChain.Shutdown(m_logicalDevice);
		m_swapChain.Init(m_logicalDevice, m_physicalDevice, m_surface);

		m_swapChain.CreateImageViews(m_logicalDevice);

		if (m_swapChain.GetImageFormat() != previousFormat)
		{
			m_renderPass.Shutdown(m_logicalDevice);
			m_renderPass.Init(m_logicalDevice, m_swapChain.GetImageFormat());

			m_graphicsPipeline.Shutdown(m_logicalDevice);
			m_pipelineLayout.Shutdown(m_logicalDevice);
			m_graphicsPipeline.Init(m_logicalDevice, m_swapChain, m_pipelineLayout, m_renderPass);
		}

		m_swapChain.CreateFramebuffers(m_logicalDevice, m_renderPass);
	}

	bool Renderer_Vulkan::CreateInstance()
	{
#if !IS_RELEASE
		if (!CheckValidationLayerSupport())
		{
			JPT_ERROR("Validation layers requested, but not available");
			return false;
		}
#endif

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Jupiter";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Jupiter Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		const DynamicArray<const char*> extensions = GetRequiredExtensions();
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32>(extensions.Count());
		createInfo.ppEnabledExtensionNames = extensions.ConstBuffer();

#if !IS_RELEASE
		createInfo.enabledLayerCount = static_cast<uint32>(validationLayers.Count());
		createInfo.ppEnabledLayerNames = validationLayers.ConstBuffer();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = m_debugMessenger.GetCreateInfo();
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
#else
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
#endif

		if (const VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create Vulkan instance! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	bool Renderer_Vulkan::CreateSurface(Window* pWindow)
	{
		WindowManager* pWindowManager = GetApplication()->GetWindowManager();
		JPT_ASSERT(pWindowManager);

		if (const VkResult result = pWindowManager->CreateSurface(pWindow, m_instance, &m_surface); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create window surface! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	bool Renderer_Vulkan::CreateCommandBuffers()
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_commandPool.Get();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32>(m_commandBuffers.Count());

		if (const VkResult result = vkAllocateCommandBuffers(m_logicalDevice.Get(), &allocInfo, m_commandBuffers.Buffer()); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to allocate command buffers! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	bool Renderer_Vulkan::CreateSyncObjects()
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < kMaxFramesInFlight; ++i)
		{
			if (const VkResult result = vkCreateSemaphore(m_logicalDevice.Get(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]); result != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create image available semaphore! VkResult: %i", static_cast<uint32>(result));
				return false;
			}
			if (const VkResult result = vkCreateSemaphore(m_logicalDevice.Get(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]); result != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create render finished semaphore! VkResult: %i", static_cast<uint32>(result));
				return false;
			}
			if (const VkResult result = vkCreateFence(m_logicalDevice.Get(), &fenceInfo, nullptr, &m_inFlightFences[i]); result != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create in flight fence! VkResult: %i", static_cast<uint32>(result));
				return false;
			}
		}

		return true;
	}

	void Renderer_Vulkan::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32 imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (const VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to begin recording command buffer! VkResult: %i", static_cast<uint32>(result));
			return;
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_renderPass.Get();
		renderPassInfo.framebuffer = m_swapChain.GetFramebuffers()[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_swapChain.GetExtent();

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline.Get());

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width  = static_cast<float>(m_swapChain.GetExtent().width);
		viewport.height = static_cast<float>(m_swapChain.GetExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor = {};	
		scissor.offset = { 0, 0 };
		scissor.extent = m_swapChain.GetExtent();
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffer);
		if (const VkResult result = vkEndCommandBuffer(commandBuffer); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to record command buffer! VkResult: %i", static_cast<uint32>(result));
		}
	}
}