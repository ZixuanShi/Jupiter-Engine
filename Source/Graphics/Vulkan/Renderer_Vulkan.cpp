// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Renderer_Vulkan;

import jpt.Asset.Manager;
import jpt.Mesh;

import jpt.Math;
import jpt.Optional;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;

import jpt.Window;
import jpt.Framework;
import jpt.Event.Manager;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;
import jpt.Event.Key;
import jpt.Event.MouseButton;
import jpt.Event.MouseMove;

import jpt.Input.Enums;
import jpt.Input.Manager;

import jpt.File.IO;
import jpt.File.Path;
import jpt.File.Path.Utils;

using namespace jpt::Vulkan;

namespace jpt
{
	bool Renderer_Vulkan::Init()
	{
		JPT_ENSURE(Super::Init());

		bool success = true;

		success &= CreateInstance();
#if !IS_RELEASE
		success &= m_debugMessenger.Init();
#endif
		success &= m_physicalDevice.Init();
		success &= m_logicalDevice.Init();

		success &= m_memoryTransferCommandPool.Init();

		success &= m_renderPass.Init(kFormat);

		success &= m_descriptorSetLayout.Init();
		success &= m_pipelineLayout.Init();
		success &= m_graphicsPipeline.Init();

		Mesh mesh;
		success &= mesh.Load(File::FixDependencies("Assets/Jupiter_Common/Meshes/Mesh_VikingRoom.obj"));
		//success &= LoadMesh(File::FixDependencies("Assets/Jupiter_Common/Meshes/Mesh_Cat.obj"));
		//success &= LoadMesh(File::FixDependencies("Assets/Jupiter_Common/Meshes/Mesh_SecurityRoom.obj"));

		success &= m_vertexBuffer.Init(mesh.GetVertices());
		success &= m_indexBuffer.Init(mesh.GetIndices());

		success &= m_descriptorPool.Init();

		success &= AssetManager::GetInstance().Load<Texture_Vulkan>(File::FixDependencies("Assets/Jupiter_Common/Textures/T_VikingRoom.png")) != nullptr;
		success &= AssetManager::GetInstance().Load<Texture_Vulkan>(File::FixDependencies("Assets/Jupiter_Common/Textures/T_Cat.png")) != nullptr;
		success &= AssetManager::GetInstance().Load<Texture_Vulkan>(File::FixDependencies("Assets/Jupiter_Common/Textures/T_SecurityRoom.png")) != nullptr;

		m_pTextureSampler = new TextureSampler_Vulkan();
		m_pTextureSampler->Init();

		// Main window
		Window* pMainWindow = GetApplication()->GetMainWindow();
		RegisterWindow(pMainWindow);

		if (!success)
		{
			JPT_ERROR("Failed to initialize Vulkan renderer");
		}

		return success;
	}

	void Renderer_Vulkan::Update(TimePrecision deltaSeconds)
	{
		Super::Update(deltaSeconds);

		for (WindowResources& resources : m_windowResources)
		{
			resources.Update(deltaSeconds);
		}
	}

	void Renderer_Vulkan::Shutdown()
	{
		m_logicalDevice.WaitIdle();

		for (WindowResources& resources : m_windowResources)
		{
			resources.Shutdown();
		}

		JPT_SHUTDOWN(m_pTextureSampler);

		m_descriptorPool.Shutdown();
		m_descriptorSetLayout.Shutdown();

		m_vertexBuffer.Shutdown();
		m_indexBuffer.Shutdown();

		m_graphicsPipeline.Shutdown();
		m_pipelineLayout.Shutdown();
		m_renderPass.Shutdown();

		m_memoryTransferCommandPool.Shutdown();

		m_logicalDevice.Shutdown();

#if !IS_RELEASE
		m_debugMessenger.Shutdown();
#endif

		vkDestroyInstance(m_instance, nullptr);

		Super::Shutdown();
	}

	void Renderer_Vulkan::DrawFrame()
	{
		Super::DrawFrame();

		for (WindowResources& resources : m_windowResources)
		{
			resources.DrawFrame();
		}
	}

	void Renderer_Vulkan::RegisterWindow(Window* pWindow)
	{
		WindowResources& resources = m_windowResources.EmplaceBack();

		resources.Init(pWindow);

		JPT_INFO("Window registered with Vulkan renderer: %lu", pWindow);
	}

	void Renderer_Vulkan::OnWindowClose(const Event_Window_Close& eventWindowClose)
	{
		const Window* pWindow = eventWindowClose.GetWindow();

		for (auto itr = m_windowResources.begin(); itr != m_windowResources.end(); ++itr)
		{
			if (itr->GetOwner() == pWindow)
			{
				itr->Shutdown();
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

	Renderer_Vulkan* GetVkRenderer()
	{
		Renderer* pRenderer = GetApplication()->GetRenderer();
		JPT_ASSERT(pRenderer != nullptr);

		Renderer_Vulkan* pVkRenderer = static_cast<Renderer_Vulkan*>(pRenderer);
		JPT_ASSERT(pVkRenderer != nullptr);

		return pVkRenderer;
	}
}