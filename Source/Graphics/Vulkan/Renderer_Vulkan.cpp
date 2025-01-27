// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"
#include "Profiling/TimingProfiler.h"

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <vector>
#include <string>
#include <unordered_map>

module jpt.Renderer_Vulkan;

import jpt.HashMap;
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

import jpt.Input.KeyCode;
import jpt.Input.Manager;

import jpt.File.IO;
import jpt.File.Path;
import jpt.File.Path.Utils;

import jpt.Math;

using namespace jpt::Vulkan;

namespace jpt
{
	bool Renderer_Vulkan::Init()
	{
		JPT_ENSURE(Super::Init());

		bool success = true;

		//GenerateSierpinski(5, { 0.5f, 0.5f, -0.25f }, { -0.5f, 0.5f, -0.25f }, { 0.0f, -0.5f, -0.25f });

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

		// Model
		success &= LoadModel();

		success &= m_vertexBuffer.Init();
		success &= m_indexBuffer.Init();

		success &= m_descriptorPool.Init();

		success &= m_texture.Load(File::FixDependencies("Assets/Jupiter_Common/Textures/T_Viking_Room.png"));

		m_pTextureSampler = new TextureSampler_Vulkan();
		m_pTextureSampler->Init();

		// Camera movement and rotation
		m_cameraMat = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		EventManager::GetInstance().Register<Event_Key>(this, &Renderer_Vulkan::OnKey);
		EventManager::GetInstance().Register<Event_Mouse_Button>(this, &Renderer_Vulkan::OnMouseButton);
		EventManager::GetInstance().Register<Event_Mouse_Move>(this, &Renderer_Vulkan::OnMouseMove);

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

		static constexpr float kCameraSpeed = 2.5f;

		const glm::vec3 forward = glm::normalize(glm::vec3(m_cameraMat[0][2], m_cameraMat[1][2], m_cameraMat[2][2]));
		const glm::vec3 right = glm::normalize(glm::vec3(m_cameraMat[0][0], m_cameraMat[1][0], m_cameraMat[2][0]));

		m_cameraMat = glm::translate(m_cameraMat, forward * m_cameraMove.z * kCameraSpeed * static_cast<float>(deltaSeconds));
		m_cameraMat = glm::translate(m_cameraMat, right * m_cameraMove.x * kCameraSpeed * static_cast<float>(deltaSeconds));

		for (WindowResources& resources : m_windowResources)
		{
			if (resources.ShouldRecreateSwapChain())
			{
				resources.RecreateSwapChain();
			}
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
		m_texture.Shutdown();

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
			if (resources.CanDraw())
			{
				resources.DrawFrame();
			}
		}
	}

	void Renderer_Vulkan::RegisterWindow(Window* pWindow)
	{
		WindowResources& resources = m_windowResources.EmplaceBack();

		resources.Init(pWindow, m_texture.GetImageView());

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

	void Renderer_Vulkan::OnKey(const Event_Key& eventKey)
	{
		Input::Key key = eventKey.GetKey();
		Input::KeyState keyState = eventKey.GetState();

		switch (key.Value())
		{
		case Input::Key::W:
			if (keyState == Input::KeyState::Pressed)
			{
				m_cameraMove.z = 1;
			}
			else if (keyState == Input::KeyState::Released)
			{
				m_cameraMove.z = 0;
			}
			break;
		case Input::Key::S:
			if (keyState == Input::KeyState::Pressed)
			{
				m_cameraMove.z = -1;
			}
			else if (keyState == Input::KeyState::Released)
			{
				m_cameraMove.z = 0;
			}
			break;
		case Input::Key::D:
			if (keyState == Input::KeyState::Pressed)
			{
				m_cameraMove.x = -1;
			}
			else if (keyState == Input::KeyState::Released)
			{
				m_cameraMove.x = 0;
			}
			break;
		case Input::Key::A:
			if (keyState == Input::KeyState::Pressed)
			{
				m_cameraMove.x = 1;
			}
			else if (keyState == Input::KeyState::Released)
			{
				m_cameraMove.x = 0;
			}
			break;
		}
	}

	void Renderer_Vulkan::OnMouseButton(const Event_Mouse_Button& eventMouseButton)
	{
		Input::MouseButton button = eventMouseButton.GetButton();
		Input::KeyState state = eventMouseButton.GetState();

		if (button == Input::MouseButton::Right)
		{
			m_isRotating = (state == Input::KeyState::Pressed);
		}
	}

	void Renderer_Vulkan::OnMouseMove([[maybe_unused]] const Event_Mouse_Move& eventMouseMove)
	{
	}
}