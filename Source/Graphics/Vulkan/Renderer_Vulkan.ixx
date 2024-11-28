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

#if IS_PLATFORM_WIN64
	import jpt.Framework_GLFW;
#endif

import jpt.Vulkan.ValidationLayers;
import jpt.Vulkan.DebugMessenger;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.Helpers;
import jpt.Vulkan.QueueFamilyIndices;
import jpt.Vulkan.SwapChainSupportDetails;

import jpt.TypeDefs;

import jpt.DynamicArray;
import jpt.StaticArray;
import jpt.HashSet;
import jpt.HashMap;

import jpt.File.IO;
import jpt.File.Enums;
import jpt.File.Path;
import jpt.File.Path.Utils;

import jpt.Event.Window.Resize;

using namespace jpt::Vulkan;

export namespace jpt
{
	constexpr size_t kMaxFramesInFlight = 2;

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
		VkDevice m_logicalDevice;

		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;

		VkSwapchainKHR m_swapChain;
		DynamicArray<VkImage> m_swapChainImages;
		VkFormat m_swapChainImageFormat;
		VkExtent2D m_swapChainExtent;
		DynamicArray<VkImageView> m_swapChainImageViews;
		DynamicArray<VkFramebuffer> m_swapChainFramebuffers;

		VkRenderPass m_renderPass;
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_graphicsPipeline;

		VkCommandPool m_commandPool;
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
		bool CreateSurface();
		bool CreateLogicalDevice();
		bool CreateSwapChain();
		bool CreateImageViews();
		bool CreateRenderPass();
		bool CreateGraphicsPipeline();
		bool CreateFramebuffers();
		bool CreateCommandPool();
		bool CreateCommandBuffers();
		bool CreateSyncObjects();

		// Vulkan helpers
		VkShaderModule CreateShaderModule(const DynamicArray<char>& code);
		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32 imageIndex);
		void RecreateSwapChain();

		// Shutdown
		void CleanupSwapChain();
	};

	bool Renderer_Vulkan::Init()
	{
		JPT_ENSURE(Super::Init());

		uint32 glfwExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &glfwExtensionCount, nullptr);
		JPT_INFO("GLFW extensions count: %i", glfwExtensionCount);

		bool success = true;
		success &= CreateInstance();
#if !IS_RELEASE
		success &= m_debugMessenger.Init(m_instance);
#endif
		success &= CreateSurface();

		success &= m_physicalDevice.Init(m_instance, m_surface);

		success &= CreateLogicalDevice();
		success &= CreateSwapChain();
		success &= CreateImageViews();
		success &= CreateRenderPass();
		success &= CreateGraphicsPipeline();
		success &= CreateFramebuffers();
		success &= CreateCommandPool();
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
		vkDeviceWaitIdle(m_logicalDevice);

		// Synchronization objects
		for (size_t i = 0; i < kMaxFramesInFlight; ++i)
		{
			vkDestroySemaphore(m_logicalDevice, m_imageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(m_logicalDevice, m_renderFinishedSemaphores[i], nullptr);
			vkDestroyFence(m_logicalDevice, m_inFlightFences[i], nullptr);
		}

		// Command buffers and pool
		vkFreeCommandBuffers(m_logicalDevice, m_commandPool,
			static_cast<uint32>(m_commandBuffers.Count()),
			m_commandBuffers.Buffer());
		vkDestroyCommandPool(m_logicalDevice, m_commandPool, nullptr);

		// Swap chain resources
		CleanupSwapChain();

		// Pipeline resources
		vkDestroyPipeline(m_logicalDevice, m_graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(m_logicalDevice, m_pipelineLayout, nullptr);

		// Device
		vkDestroyDevice(m_logicalDevice, nullptr);

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
			- Record a command buffer which draws the scene onto that image
			- Submit the recorded command buffer
			- Present the swap chain image	*/

		vkWaitForFences(m_logicalDevice, 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

		uint32 imageIndex = 0;
		const VkResult resultAcquireNextImage = vkAcquireNextImageKHR(m_logicalDevice, m_swapChain, UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);
		if (resultAcquireNextImage == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return;
		}
		JPT_ASSERT(resultAcquireNextImage == VK_SUCCESS || resultAcquireNextImage == VK_SUBOPTIMAL_KHR, "failed to acquire swap chain image %u", static_cast<uint32>(resultAcquireNextImage));
		
		vkResetFences(m_logicalDevice, 1, &m_inFlightFences[m_currentFrame]);

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

		if (const VkResult result = vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, m_inFlightFences[m_currentFrame]); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to submit draw command buffer! VkResult: %i", static_cast<uint32>(result));
			return;
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		const VkResult resultQueuePresent = vkQueuePresentKHR(m_presentQueue, &presentInfo);
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

	void Renderer_Vulkan::OnWindowCreate(const Event_Window_Create& eventWindowCreate)
	{
		JPT_IGNORE(eventWindowCreate);
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
		vkDeviceWaitIdle(m_logicalDevice);

		CleanupSwapChain();

		CreateSwapChain();
		CreateImageViews();
		CreateRenderPass();
		CreateFramebuffers();
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

	bool Renderer_Vulkan::CreateSurface()
	{
		if (const VkResult result = Framework_GLFW::CreateWindowSurface(m_instance, &m_surface); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create window surface! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	bool Renderer_Vulkan::CreateLogicalDevice()
	{
		DynamicArray<VkDeviceQueueCreateInfo> queueCreateInfos;
		const QueueFamilyIndices& queueFamilyIndices = m_physicalDevice.GetQueueFamilyIndices();
		HashSet<uint32> uniqueQueueFamilies = { queueFamilyIndices.graphicsFamily.Value(), queueFamilyIndices.presentFamily.Value()};

		float queuePriority = 1.0f;
		for (uint32 queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			queueCreateInfos.Add(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32>(queueCreateInfos.Count());
		createInfo.pQueueCreateInfos = queueCreateInfos.ConstBuffer();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32>(deviceExtensions.Count());
		createInfo.ppEnabledExtensionNames = deviceExtensions.ConstBuffer();

#if !IS_RELEASE
		createInfo.enabledLayerCount = static_cast<uint32>(validationLayers.Count());
		createInfo.ppEnabledLayerNames = validationLayers.ConstBuffer();
#else
		createInfo.enabledLayerCount = 0;
#endif

		if (const VkResult result = vkCreateDevice(m_physicalDevice.Get(), &createInfo, nullptr, &m_logicalDevice); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create logical device! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		vkGetDeviceQueue(m_logicalDevice, queueFamilyIndices.graphicsFamily.Value(), 0, &m_graphicsQueue);
		vkGetDeviceQueue(m_logicalDevice, queueFamilyIndices.presentFamily.Value(), 0, &m_presentQueue);
		return true;
	}

	bool Renderer_Vulkan::CreateSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = m_physicalDevice.QuerySwapChainSupport(m_physicalDevice.Get(), m_surface);

		const VkSurfaceFormatKHR surfaceFormat = swapChainSupport.ChooseSwapSurfaceFormat();
		const VkPresentModeKHR presentMode = swapChainSupport.ChooseSwapPresentMode();
		const VkExtent2D extent = swapChainSupport.ChooseSwapExtent();

		uint32 imageCount = swapChainSupport.GetImageCount();

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		const QueueFamilyIndices& queueFaimilyIndices = m_physicalDevice.GetQueueFamilyIndices();
		uint32 indices[] = { queueFaimilyIndices.graphicsFamily.Value(), queueFaimilyIndices.presentFamily.Value() };

		if (queueFaimilyIndices.graphicsFamily != queueFaimilyIndices.presentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = indices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapChainSupport.GetTransform();
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (const VkResult result = vkCreateSwapchainKHR(m_logicalDevice, &createInfo, nullptr, &m_swapChain); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create swap chain! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &imageCount, nullptr);
		m_swapChainImages.Resize(imageCount);
		vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &imageCount, m_swapChainImages.Buffer());

		m_swapChainImageFormat = surfaceFormat.format;
		m_swapChainExtent = extent;

		return true;
	}

	bool Renderer_Vulkan::CreateImageViews()
	{
		m_swapChainImageViews.Resize(m_swapChainImages.Count());

		for (uint32 i = 0; i < m_swapChainImages.Count(); ++i)
		{
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_swapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (const VkResult result = vkCreateImageView(m_logicalDevice, &createInfo, nullptr, &m_swapChainImageViews[i]); result != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create image views! VkResult: %i", static_cast<uint32>(result));
				return false;
			}
		}

		return true;
	}

	bool Renderer_Vulkan::CreateRenderPass()
	{
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = m_swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (const VkResult result = vkCreateRenderPass(m_logicalDevice, &renderPassInfo, nullptr, &m_renderPass); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create render pass! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	bool Renderer_Vulkan::CreateGraphicsPipeline()
	{
		const File::Path vertexShaderPath = File::FixDependencies("_Baked/Jupiter_Common/Shaders/triangle.vs.spv");
		const File::Path pixelShaderPath = File::FixDependencies("_Baked/Jupiter_Common/Shaders/triangle.ps.spv");

		JPT_ASSERT(File::Exists(vertexShaderPath), "Failed finding vertex shader in %ls, did you compile shaders from Engine's Scripts/CompileShaders.bat?", vertexShaderPath.ConstBuffer());
		JPT_ASSERT(File::Exists(pixelShaderPath), "Failed finding pixel shader in %ls, did you compile shaders from Engine's Scripts/CompileShaders.bat?", pixelShaderPath.ConstBuffer());

		DynamicArray<char> vertexShaderCode = ReadBinaryFileArray(vertexShaderPath);
		DynamicArray<char> pixelShaderCode = ReadBinaryFileArray(pixelShaderPath);

		VkShaderModule vertexShaderModule = CreateShaderModule(vertexShaderCode);
		VkShaderModule pixelShaderModule = CreateShaderModule(pixelShaderCode);

		VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
		vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertexShaderStageInfo.module = vertexShaderModule;
		vertexShaderStageInfo.pName = "main";
		vertexShaderStageInfo.pSpecializationInfo = nullptr;	// Constants optimization

		VkPipelineShaderStageCreateInfo pixelShaderStageInfo = {};
		pixelShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pixelShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		pixelShaderStageInfo.module = pixelShaderModule;
		pixelShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, pixelShaderStageInfo };

		// Vertex input
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;

		// Input assembly
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		// Viewport
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_swapChainExtent.width);
		viewport.height = static_cast<float>(m_swapChainExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		// Scissor
		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = m_swapChainExtent;

		// Dynamic states
		DynamicArray<VkDynamicState> dynamicStates = 
		{
			VK_DYNAMIC_STATE_VIEWPORT, 
			VK_DYNAMIC_STATE_SCISSOR,
			VK_DYNAMIC_STATE_LINE_WIDTH 
		};
		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32>(dynamicStates.Count());
		dynamicState.pDynamicStates = dynamicStates.ConstBuffer();

		// Viewport state
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;
		viewportState.pViewports = nullptr;
		viewportState.pScissors = nullptr;

		// Rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		// Multisampling
		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		// Depth and stencil testing
		// Later

		// Color blending
		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;

		// Pipeline layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		
		if (const VkResult result = vkCreatePipelineLayout(m_logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create pipeline layout! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		// Create pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = m_pipelineLayout;
		pipelineInfo.renderPass = m_renderPass;
		pipelineInfo.subpass = 0;

		if (const VkResult result = vkCreateGraphicsPipelines(m_logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create graphics pipeline! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		vkDestroyShaderModule(m_logicalDevice, vertexShaderModule, nullptr);
		vkDestroyShaderModule(m_logicalDevice, pixelShaderModule, nullptr);

		return true;
	}

	bool Renderer_Vulkan::CreateFramebuffers()
	{
		m_swapChainFramebuffers.Resize(m_swapChainImageViews.Count());

		for (size_t i = 0; i < m_swapChainImageViews.Count(); ++i)
		{
			VkImageView attachments[] = { m_swapChainImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = m_swapChainExtent.width;
			framebufferInfo.height = m_swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (const VkResult result = vkCreateFramebuffer(m_logicalDevice, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]); result != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create framebuffer! VkResult: %i", static_cast<uint32>(result));
				return false;
			}
		}

		return true;
	}

	bool Renderer_Vulkan::CreateCommandPool()
	{
		const QueueFamilyIndices& queueFamilyIndices = m_physicalDevice.GetQueueFamilyIndices();

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.Value();

		if (const VkResult result = vkCreateCommandPool(m_logicalDevice, &poolInfo, nullptr, &m_commandPool); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create command pool! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	bool Renderer_Vulkan::CreateCommandBuffers()
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32>(m_commandBuffers.Count());

		if (const VkResult result = vkAllocateCommandBuffers(m_logicalDevice, &allocInfo, m_commandBuffers.Buffer()); result != VK_SUCCESS)
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
			if (const VkResult result = vkCreateSemaphore(m_logicalDevice, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]); result != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create image available semaphore! VkResult: %i", static_cast<uint32>(result));
				return false;
			}
			if (const VkResult result = vkCreateSemaphore(m_logicalDevice, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]); result != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create render finished semaphore! VkResult: %i", static_cast<uint32>(result));
				return false;
			}
			if (const VkResult result = vkCreateFence(m_logicalDevice, &fenceInfo, nullptr, &m_inFlightFences[i]); result != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create in flight fence! VkResult: %i", static_cast<uint32>(result));
				return false;
			}
		}

		return true;
	}

	VkShaderModule Renderer_Vulkan::CreateShaderModule(const DynamicArray<char>& code)
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.Count();
		createInfo.pCode = reinterpret_cast<const uint32*>(code.ConstBuffer());

		VkShaderModule shaderModule;
		if (const VkResult result = vkCreateShaderModule(m_logicalDevice, &createInfo, nullptr, &shaderModule); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create shader module! VkResult: %i", static_cast<uint32>(result));
		}

		return shaderModule;
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
		renderPassInfo.renderPass = m_renderPass;
		renderPassInfo.framebuffer = m_swapChainFramebuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_swapChainExtent;

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_swapChainExtent.width);
		viewport.height = static_cast<float>(m_swapChainExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor = {};	
		scissor.offset = { 0, 0 };
		scissor.extent = m_swapChainExtent;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffer);
		if (const VkResult result = vkEndCommandBuffer(commandBuffer); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to record command buffer! VkResult: %i", static_cast<uint32>(result));
		}
	}

	void Renderer_Vulkan::CleanupSwapChain()
	{
		// Wait for device to finish operations
		vkDeviceWaitIdle(m_logicalDevice);

		// Destroy framebuffers
		for (VkFramebuffer framebuffer : m_swapChainFramebuffers)
		{
			vkDestroyFramebuffer(m_logicalDevice, framebuffer, nullptr);
		}
		m_swapChainFramebuffers.Clear();

		// Destroy image views
		for (VkImageView imageView : m_swapChainImageViews)
		{
			vkDestroyImageView(m_logicalDevice, imageView, nullptr);
		}
		m_swapChainImageViews.Clear();

		// Destroy render pass and swap chain
		vkDestroyRenderPass(m_logicalDevice, m_renderPass, nullptr);
		vkDestroySwapchainKHR(m_logicalDevice, m_swapChain, nullptr);
	}
}