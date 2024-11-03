// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

export module jpt.Renderer_Vulkan;

import jpt.Renderer;

import jpt.Vulkan.ValidationLayers;

import jpt.TypeDefs;

export namespace jpt
{
	class Renderer_Vulkan final : public Renderer
	{
		using Super = Renderer;

	private:
		VkInstance m_instance;

	public:
		virtual bool Init() override;
		virtual void Shutdown() override;
	};

	bool Renderer_Vulkan::Init()
	{
		JPT_ENSURE(Super::Init());

		if constexpr (kEnableValidationLayers)
		{
			if (!CheckValidationLayerSupport())
			{
				JPT_ERROR("Validation layers requested, but not available");
				return false;
			}
		}

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Jupiter";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Jupiter Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		if constexpr (kEnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32>(validationLayers.Count());
			createInfo.ppEnabledLayerNames = validationLayers.ConstBuffer();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
		if (result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create Vulkan instance! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		JPT_INFO("Vulkan instance created successfully");
		return true;
	}

	void Renderer_Vulkan::Shutdown()
	{
		Super::Shutdown();

		vkDestroyInstance(m_instance, nullptr);
	}
}