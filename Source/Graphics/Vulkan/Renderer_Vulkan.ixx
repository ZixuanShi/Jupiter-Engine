// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"
#include "Core/Validation/Assert.h"

#include <vulkan/vulkan.h>

export module jpt.Renderer_Vulkan;

import jpt.Renderer;
import jpt.Graphics.Constants;

import jpt.Vulkan.Constants;
import jpt.Vulkan.WindowResources;
import jpt.Vulkan.Extensions;
import jpt.Vulkan.ValidationLayers;
import jpt.Vulkan.DebugMessenger;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.PipelineLayout;
import jpt.Vulkan.GraphicsPipeline;
import jpt.Vulkan.RenderPass;

import jpt.DynamicArray;
import jpt.TypeDefs;

import jpt.Window;
import jpt.Framework;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;

using namespace jpt::Vulkan;

export namespace jpt
{
	class Renderer_Vulkan final : public Renderer
	{
		using Super = Renderer;

	private:
		VkInstance m_instance = VK_NULL_HANDLE;
#if !IS_RELEASE
		DebugMessenger m_debugMessenger;
#endif
		PhysicalDevice m_physicalDevice;
		LogicalDevice m_logicalDevice;

		RenderPass m_renderPass;
		PipelineLayout m_pipelineLayout;
		GraphicsPipeline m_graphicsPipeline;

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

		bool success = true;

		success &= CreateInstance();
#if !IS_RELEASE
		success &= m_debugMessenger.Init(m_instance);
#endif
		success &= m_physicalDevice.Init(m_instance);
		success &= m_logicalDevice.Init(m_physicalDevice);
		success &= m_renderPass.Init(m_logicalDevice, kFormat);
		success &= m_pipelineLayout.Init(m_logicalDevice);
		success &= m_graphicsPipeline.Init(m_logicalDevice, m_pipelineLayout, m_renderPass);

		// Main window
		Window* pMainWindow = GetApplication()->GetMainWindow();
		RegisterWindow(pMainWindow);

		return success;
	}

	void Renderer_Vulkan::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);
	}

	void Renderer_Vulkan::Shutdown()
	{
		m_logicalDevice.WaitIdle();

		for (WindowResources& resources : m_windowResources)
		{
			resources.Shutdown(m_instance, m_logicalDevice);
		}

		m_graphicsPipeline.Shutdown(m_logicalDevice);
		m_pipelineLayout.Shutdown(m_logicalDevice);
		m_renderPass.Shutdown(m_logicalDevice);
		m_logicalDevice.Shutdown();

#if !IS_RELEASE
		m_debugMessenger.Shutdown(m_instance);
#endif

		vkDestroyInstance(m_instance, nullptr);

		Super::Shutdown();
	}

	void Renderer_Vulkan::DrawFrame()
	{
		Super::DrawFrame();

		for (WindowResources& resources : m_windowResources)
		{
			resources.DrawFrame(m_logicalDevice, m_renderPass, m_graphicsPipeline);
		}
	}

	void Renderer_Vulkan::RegisterWindow(Window* pWindow)
	{
		WindowResources& resources = m_windowResources.EmplaceBack();
		resources.Init(pWindow, m_instance, m_physicalDevice, m_logicalDevice, m_renderPass);

		JPT_INFO("Window registered with Vulkan renderer: %lu", pWindow);
	}

	void Renderer_Vulkan::OnWindowResize(const Event_Window_Resize&  )
	{
	}

	void Renderer_Vulkan::OnWindowClose(const Event_Window_Close& eventWindowClose)
	{
		const Window* pWindow = eventWindowClose.GetWindow();

		for (auto itr = m_windowResources.begin(); itr != m_windowResources.end(); ++itr)
		{
			if (itr->GetOwner() == pWindow)
			{
				itr->Shutdown(m_instance, m_logicalDevice);
				m_windowResources.Erase(itr);
				break;
			}
		}

		JPT_INFO("Window unregistered with Vulkan renderer: %lu", pWindow);
	}

	bool Renderer_Vulkan::CreateInstance()
	{
#if !IS_RELEASE
		if (!CheckValidationLayerSupport())
		{
			JPT_ERROR("Validation layers requested, but not available!");
			return false;
		}
#endif

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Jupiter Engine";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Jupiter Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		DynamicArray<const char*> extensions = GetRequiredExtensions();
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32>(extensions.Count());
		createInfo.ppEnabledExtensionNames = extensions.ConstBuffer();

#if !IS_RELEASE
		createInfo.enabledLayerCount = static_cast<uint32>(g_validationLayers.Count());
		createInfo.ppEnabledLayerNames = g_validationLayers.ConstBuffer();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = m_debugMessenger.MakeCreateInfo();
		createInfo.pNext = &debugCreateInfo;
#else
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
#endif

		if (const VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create Vulkan instance: %d", result);
			return false;
		}

		return true;
	}
}