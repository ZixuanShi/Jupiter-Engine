// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

export module jpt.Renderer_Vulkan;

import jpt.Renderer;

import jpt.Vulkan.ValidationLayers;

import jpt.DynamicArray;
import jpt.TypeDefs;

export namespace jpt
{
	class Renderer_Vulkan final : public Renderer
	{
		using Super = Renderer;

	private:
		VkInstance m_instance;
		VkDebugUtilsMessengerEXT m_debugMessenger;

	public:
		virtual bool Init() override;
		virtual void Shutdown() override;

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
			                                                VkDebugUtilsMessageTypeFlagsEXT messageType, 
			                                                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
			                                                void* pUserData);

	private:
		bool CreateInstance();

		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		bool SetupDebugMessenger();

		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
			                                  const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			                                  const VkAllocationCallbacks* pAllocator,
			                                  VkDebugUtilsMessengerEXT* pCallback);
		void DestroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);
	};

	bool Renderer_Vulkan::Init()
	{
		JPT_ENSURE(Super::Init());

		if (!CreateInstance())
		{
			return false;
		}

		if (!SetupDebugMessenger())
		{
			return false;
		}

		return true;
	}

	void Renderer_Vulkan::Shutdown()
	{
		Super::Shutdown();

		if constexpr (kEnableValidationLayers)
		{
			DestroyDebugMessenger(m_instance, m_debugMessenger, nullptr);
		}
		vkDestroyInstance(m_instance, nullptr);
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL Renderer_Vulkan::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		                                                          [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
		                                                          const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		                                                          [[maybe_unused]] void* pUserData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: 
			[[fallthrough]];
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			JPT_INFO("Validation layer: %s", pCallbackData->pMessage);
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			JPT_WARNING("Validation layer: %s", pCallbackData->pMessage);
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			JPT_ERROR("Validation layer: %s", pCallbackData->pMessage);
			break;

		default:
			break;
		}

		return VK_FALSE;
	}

	bool Renderer_Vulkan::CreateInstance()
	{
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

		const DynamicArray<const char*> extensions = GetRequiredExtensions();
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32>(extensions.Count());
		createInfo.ppEnabledExtensionNames = extensions.ConstBuffer();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if constexpr (kEnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32>(validationLayers.Count());
			createInfo.ppEnabledLayerNames = validationLayers.ConstBuffer();

			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
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

	void Renderer_Vulkan::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
			                     VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			                     VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr; // Optional
	}

	bool Renderer_Vulkan::SetupDebugMessenger()
	{
		if constexpr (!kEnableValidationLayers)
		{
			return true;
		}

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
		{
			JPT_ERROR("Failed to set up debug messenger");
			return false;
		}

		JPT_INFO("Debug messenger set up successfully");
		return true;
	}

	VkResult Renderer_Vulkan::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void Renderer_Vulkan::DestroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, callback, pAllocator);
		}
	}
}