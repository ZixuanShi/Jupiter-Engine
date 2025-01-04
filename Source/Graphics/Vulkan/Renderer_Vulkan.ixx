// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"
#include "Profiling/TimingProfiler.h"

#include <vulkan/vulkan.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <vector>
#include <string>
#include <unordered_map>

export module jpt.Renderer_Vulkan;

import jpt.Renderer;
import jpt.Graphics.Constants;

import jpt.Vertex;
import jpt.Vulkan.Data;
import jpt.Vulkan.Utils;

import jpt.Vulkan.Constants;
import jpt.Vulkan.WindowResources;
import jpt.Vulkan.Extensions;
import jpt.Vulkan.ValidationLayers;
import jpt.Vulkan.DebugMessenger;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.RenderPass;
import jpt.Vulkan.DescriptorSetLayout;
import jpt.Vulkan.DescriptorPool;
import jpt.Vulkan.PipelineLayout;
import jpt.Vulkan.GraphicsPipeline;
import jpt.Vulkan.VertexBuffer;
import jpt.Vulkan.IndexBuffer;
import jpt.Vulkan.Texture;
import jpt.Vulkan.Texture.Sampler;

import jpt.DynamicArray;
import jpt.HashMap;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;

import jpt.Window;
import jpt.Framework;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;

import jpt.File.IO;
import jpt.File.Path;
import jpt.File.Path.Utils;

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

		DescriptorSetLayout m_descriptorSetLayout;
		DescriptorPool m_descriptorPool;

		RenderPass m_renderPass;
		PipelineLayout m_pipelineLayout;
		GraphicsPipeline m_graphicsPipeline;

		VertexBuffer m_vertexBuffer;
		IndexBuffer m_indexBuffer;

		// Texture
		Texture_Vulkan m_texture;

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

		bool LoadModel();
	};

	bool Renderer_Vulkan::Init()
	{
		JPT_ENSURE(Super::Init());

		bool success = true;

		//GenerateSierpinski(5, { 0.5f, 0.5f, -0.25f }, { -0.5f, 0.5f, -0.25f }, { 0.0f, -0.5f, -0.25f });

		success &= CreateInstance();
#if !IS_RELEASE
		success &= m_debugMessenger.Init(m_instance);
#endif
		success &= m_physicalDevice.Init(m_instance);
		success &= m_logicalDevice.Init(m_physicalDevice);

		success &= m_memoryTransferCommandPool.Init(m_logicalDevice, m_physicalDevice.GetGraphicsFamilyIndex());

		success &= m_renderPass.Init(m_physicalDevice, m_logicalDevice, kFormat);

		success &= m_descriptorSetLayout.Init(m_logicalDevice);
		success &= m_pipelineLayout.Init(m_logicalDevice, m_descriptorSetLayout);
		success &= m_graphicsPipeline.Init(m_logicalDevice, m_pipelineLayout, m_renderPass);

		// Model
		success &= LoadModel();

		success &= m_vertexBuffer.Init(m_physicalDevice, m_logicalDevice, m_memoryTransferCommandPool);
		success &= m_indexBuffer.Init(m_physicalDevice, m_logicalDevice, m_memoryTransferCommandPool);

		success &= m_descriptorPool.Init(m_logicalDevice);

		success &= m_texture.Init(m_physicalDevice, m_logicalDevice, m_memoryTransferCommandPool);
		success &= m_texture.Load(File::FixDependencies("Assets/Jupiter_Common/Textures/T_Viking_Room.png"));

		m_pTextureSampler = new TextureSampler_Vulkan(m_physicalDevice, m_logicalDevice);
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
			if (resources.ShouldRecreateSwapChain())
			{
				resources.RecreateSwapChain(m_physicalDevice, m_logicalDevice, m_renderPass);
			}
		}
	}

	void Renderer_Vulkan::Shutdown()
	{
		m_logicalDevice.WaitIdle();

		for (WindowResources& resources : m_windowResources)
		{
			resources.Shutdown(m_instance, m_logicalDevice);
		}

		JPT_SHUTDOWN(m_pTextureSampler);
		m_texture.Shutdown();

		m_descriptorPool.Shutdown(m_logicalDevice);
		m_descriptorSetLayout.Shutdown(m_logicalDevice);

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
			m_descriptorSetLayout, m_descriptorPool,
			m_texture.GetImageView());

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

	bool Renderer_Vulkan::LoadModel()
	{
		File::Path modelPath = File::FixDependencies("Assets/Jupiter_Common/Models/Model_Viking_Room.obj");

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.ToCString().ConstBuffer()))
		{
			JPT_ERROR("Failed to load model: %s", err.c_str());
			return false;
		}

		{
			JPT_SCOPED_TIMING_PROFILER("Load model");

			//std::unordered_map<Vertex, uint32> uniqueVertices;
			HashMap<Vertex, uint32> uniqueVertices;

			for (const tinyobj::shape_t& shape : shapes)
			{
				for (const tinyobj::index_t& index : shape.mesh.indices)
				{
					Vertex vertex = {};

					vertex.position.x = attrib.vertices[3 * index.vertex_index + 0];
					vertex.position.y = attrib.vertices[3 * index.vertex_index + 1];
					vertex.position.z = attrib.vertices[3 * index.vertex_index + 2];

					vertex.texCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
					vertex.texCoord.y = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1];

					vertex.color = { 1.0f, 1.0f, 1.0f };

					//g_vertices.Add(vertex);
					//g_indices.Add(static_cast<uint32>(g_indices.Count()));

					if (!uniqueVertices.Has(vertex))
					{
						uniqueVertices[vertex] = static_cast<uint32>(g_vertices.Count());
						g_vertices.EmplaceBack(vertex);
					}

					g_indices.EmplaceBack(uniqueVertices[vertex]);
				}
			}

			JPT_LOG("Vertices: %i", g_vertices.Count());
			JPT_LOG("Indices: %i", g_indices.Count());
		}

		return true;
	}
}