// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"
#include "Core/Validation/Assert.h"

#include <vulkan/vulkan.h>

export module jpt.Renderer_Vulkan;

import jpt.Renderer;

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
import jpt.Vulkan.CommandBuffers;
import jpt.Vulkan.SynchronizationObjects;
import jpt.Vulkan.Helpers;
import jpt.Vulkan.QueueFamilyIndices;
import jpt.Vulkan.SwapChainSupportDetails;

import jpt.TypeDefs;
import jpt.Time.TypeDefs;
import jpt.DynamicArray;

import jpt.Window;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;

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

		PhysicalDevice m_physicalDevice;
		LogicalDevice m_logicalDevice;

		RenderPass m_renderPass;

		PipelineLayout m_pipelineLayout;
		Pipeline m_graphicsPipeline;

		DynamicArray<WindowResources> m_windowResources;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

		virtual void DrawFrame() override;

		virtual void RegisterWindow(Window* pWindow) override;
		virtual void OnWindowResize(const Event_Window_Resize& eventWindowResize) override;
		virtual void OnWindowClose(const Event_Window_Close& eventWindowClose) override;

	private:
		bool CreateInstance();
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

		// Main window
		Window* pMainWindow = GetApplication()->GetMainWindow();
		WindowResources& resources = m_windowResources.EmplaceBack();
		resources.SetOwner(pMainWindow);

		// Surface
		success &= pMainWindow->CreateSurface({ m_instance, resources.GetSurfacePtr() });

		// Physical and logical devices
		success &= m_physicalDevice.Init(m_instance, resources.GetSurface());
		success &= m_logicalDevice.Init(m_physicalDevice);
		resources.SetLogicalDevice(&m_logicalDevice);

		// Swap chain
		success &= resources.CreateSwapChain(m_physicalDevice);
		success &= resources.CreateImageViews();

		// Render pass
		success &= m_renderPass.Init(m_logicalDevice, resources.GetImageFormat());

		// Pipeline resources
		success &= m_pipelineLayout.Init(m_logicalDevice);
		success &= m_graphicsPipeline.Init(m_logicalDevice, resources.GetSwapChain(), m_pipelineLayout, m_renderPass);

		// Framebuffers
		success &= resources.CreateFramebuffers(m_renderPass);

		// Command pool and buffers
		const uint32 queueFamilyIndex = m_physicalDevice.GetQueueFamilyIndices().graphicsFamily.Value();
		success &= resources.CreateCommandPool(queueFamilyIndex);
		success &= resources.CreateCommandBuffers();

		// Synchronization objects
		success &= resources.CreateSynchronizationObjects();

		if (success)
		{
			JPT_INFO("Vulkan renderer initialized successfully");
		}

		return success;
	}

	void Renderer_Vulkan::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		for (WindowResources& resources : m_windowResources)
		{
			if (resources.ShouldRecreateSwapChain())
			{
				resources.RecreateSwapChain(m_physicalDevice, m_renderPass, m_graphicsPipeline, m_pipelineLayout);
			}
		}
	}

	void Renderer_Vulkan::Shutdown()
	{
		m_logicalDevice.WaitIdle();

		// Window resources
		for (WindowResources& resources : m_windowResources)
		{
			resources.Shutdown(m_instance);
		}
		m_windowResources.Clear();

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
		vkDestroyInstance(m_instance, nullptr);

		Super::Shutdown();
	}

	void Renderer_Vulkan::DrawFrame()
	{
		Super::DrawFrame();

		for (WindowResources& resources : m_windowResources)
		{
			if (resources.CanDraw())
			{
				resources.DrawFrame(m_renderPass, m_graphicsPipeline);
			}
		}
	}

	void Renderer_Vulkan::RegisterWindow(Window* pWindow)
	{
		WindowResources& resources = m_windowResources.EmplaceBack();
		resources.SetOwner(pWindow);
		resources.SetLogicalDevice(&m_logicalDevice);

		// Surface
		pWindow->CreateSurface({ m_instance, resources.GetSurfacePtr()});

		// Swap chain
		resources.CreateSwapChain(m_physicalDevice);
		resources.CreateImageViews();

		// Framebuffers
		resources.CreateFramebuffers(m_renderPass);

		// Command pool and buffers
		const uint32 queueFamilyIndex = m_physicalDevice.GetQueueFamilyIndices().graphicsFamily.Value();
		resources.CreateCommandPool(queueFamilyIndex);
		resources.CreateCommandBuffers();

		// Synchronization objects
		resources.CreateSynchronizationObjects();

		JPT_INFO("Window registered with Vulkan renderer: %lu", pWindow);
	}

	void Renderer_Vulkan::OnWindowResize(const Event_Window_Resize& eventWindowResize)
	{
		if (eventWindowResize.IsMinimized())
		{
			return;
		}

		for (WindowResources& resources : m_windowResources)
		{
			if (resources.GetOwner() == eventWindowResize.GetWindow())
			{
				resources.RecreateSwapChain(m_physicalDevice, m_renderPass, m_graphicsPipeline, m_pipelineLayout);
				break;
			}
		}
	}

	void Renderer_Vulkan::OnWindowClose(const Event_Window_Close& eventWindowClose)
	{
		for (auto itr = m_windowResources.begin(); itr != m_windowResources.end(); ++itr)
		{
			if (itr->GetOwner() == eventWindowClose.GetWindow())
			{
				itr->Shutdown(m_instance);
				m_windowResources.Erase(itr);
				break;
			}
		}
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
}