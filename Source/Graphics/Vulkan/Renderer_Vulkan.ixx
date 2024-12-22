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

import jpt.Vulkan.Vertex;
import jpt.Vulkan.Utils;

import jpt.Vulkan.Constants;
import jpt.Vulkan.WindowResources;
import jpt.Vulkan.Extensions;
import jpt.Vulkan.ValidationLayers;
import jpt.Vulkan.DebugMessenger;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.PipelineLayout;
import jpt.Vulkan.GraphicsPipeline;
import jpt.Vulkan.RenderPass;
import jpt.Vulkan.VertexBuffer;
import jpt.Vulkan.IndexBuffer;

import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;

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

		CommandPool m_memoryTransferCommandPool;

		// Descriptor set layout
		VkDescriptorSetLayout m_descriptorSetLayout;

		// Descriptor Pool
		VkDescriptorPool m_descriptorPool;

		RenderPass m_renderPass;
		PipelineLayout m_pipelineLayout;
		GraphicsPipeline m_graphicsPipeline;

		VertexBuffer m_vertexBuffer;
		IndexBuffer m_indexBuffer;

		DynamicArray<WindowResources> m_windowResources;

	public:
		virtual bool Init() override;
		virtual void Update(TimePrecision deltaSeconds) override;
		virtual void Shutdown() override;

		virtual void DrawFrame() override;

		virtual void RegisterWindow(Window* pWindow) override;
		virtual void OnWindowClose(const Event_Window_Close& eventWindowClose) override;

	private:
		bool CreateInstance();
	};

	bool Renderer_Vulkan::Init()
	{
		JPT_ENSURE(Super::Init());

		bool success = true;

		GenerateSierpinski(5, { 0.5f, 0.5f }, { -0.5f, 0.5f }, { 0.0f, -0.5f });

		success &= CreateInstance();
#if !IS_RELEASE
		success &= m_debugMessenger.Init(m_instance);
#endif
		success &= m_physicalDevice.Init(m_instance);
		success &= m_logicalDevice.Init(m_physicalDevice);

		success &= m_memoryTransferCommandPool.Init(m_logicalDevice, m_physicalDevice.GetGraphicsFamilyIndex());

		success &= m_renderPass.Init(m_logicalDevice, kFormat);

		// Descriptor set layout
		{
			VkDescriptorSetLayoutBinding uboLayoutBinding{};
			uboLayoutBinding.binding = 0;
			uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			uboLayoutBinding.descriptorCount = 1;
			uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

			VkDescriptorSetLayoutCreateInfo layoutInfo{};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = 1;
			layoutInfo.pBindings = &uboLayoutBinding;

			if (const VkResult result = vkCreateDescriptorSetLayout(m_logicalDevice.GetHandle(), &layoutInfo, nullptr, &m_descriptorSetLayout); result != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create descriptor set layout: %d", result);
				success = false;
			}
		}

		success &= m_pipelineLayout.Init(m_logicalDevice, m_descriptorSetLayout);
		success &= m_graphicsPipeline.Init(m_logicalDevice, m_pipelineLayout, m_renderPass);

		success &= m_vertexBuffer.Init(m_physicalDevice, m_logicalDevice, m_memoryTransferCommandPool);
		success &= m_indexBuffer.Init(m_physicalDevice, m_logicalDevice, m_memoryTransferCommandPool);

		// Descriptor Pool
		{
			VkDescriptorPoolSize poolSize{};
			poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSize.descriptorCount = static_cast<uint32>(kMaxFramesInFlight);

			VkDescriptorPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = 1;
			poolInfo.pPoolSizes = &poolSize;
			poolInfo.maxSets = static_cast<uint32>(kMaxFramesInFlight);

			if (const VkResult result = vkCreateDescriptorPool(m_logicalDevice.GetHandle(), &poolInfo, nullptr, &m_descriptorPool); result != VK_SUCCESS)
			{
				JPT_ERROR("Failed to create descriptor pool: %d", result);
				success = false;
			}
		}

		// Main window
		Window* pMainWindow = GetApplication()->GetMainWindow();
		RegisterWindow(pMainWindow);

		return success;
	}

	void Renderer_Vulkan::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		for (WindowResources& resources : m_windowResources)
		{
			if (resources.ShouldRecreateSwapChain())
			{
				resources.RecreateSwapChain(m_physicalDevice, m_logicalDevice, m_renderPass);
			}
		}
	}

	void Renderer_Vulkan::Shutdown()
	{
		m_logicalDevice.WaitIdle();

		// Descriptor Pool
		vkDestroyDescriptorPool(m_logicalDevice.GetHandle(), m_descriptorPool, nullptr);

		// Descriptor set layout
		vkDestroyDescriptorSetLayout(m_logicalDevice.GetHandle(), m_descriptorSetLayout, nullptr);

		for (WindowResources& resources : m_windowResources)
		{
			resources.Shutdown(m_instance, m_logicalDevice);
		}

		m_vertexBuffer.Shutdown(m_logicalDevice);
		m_indexBuffer.Shutdown(m_logicalDevice);

		m_graphicsPipeline.Shutdown(m_logicalDevice);
		m_pipelineLayout.Shutdown(m_logicalDevice);
		m_renderPass.Shutdown(m_logicalDevice);

		m_memoryTransferCommandPool.Shutdown(m_logicalDevice);

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
			if (resources.CanDraw())
			{
				resources.DrawFrame(m_logicalDevice, m_renderPass, m_pipelineLayout, m_graphicsPipeline, 
					m_vertexBuffer, m_indexBuffer);
			}
		}
	}

	void Renderer_Vulkan::RegisterWindow(Window* pWindow)
	{
		WindowResources& resources = m_windowResources.EmplaceBack();

		resources.Init(pWindow, m_instance, 
			m_physicalDevice, m_logicalDevice, m_renderPass,
			m_descriptorSetLayout, m_descriptorPool);

		JPT_INFO("Window registered with Vulkan renderer: %lu", pWindow);
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
		appInfo.pApplicationName = GetApplication()->GetName();
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