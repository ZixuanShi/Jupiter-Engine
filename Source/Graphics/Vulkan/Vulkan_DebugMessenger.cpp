// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan.DebugMessenger;

import jpt.Application;
import jpt.Renderer_Vulkan;

namespace jpt::Vulkan
{
	bool DebugMessenger::Init()
	{
		const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
		const VkInstance vkInstance = pVulkanRenderer->GetVkInstance();

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkInstance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			VkDebugUtilsMessengerCreateInfoEXT createInfo = MakeCreateInfo();
			return func(vkInstance, &createInfo, nullptr, &m_debugMessenger) == VkResult::VK_SUCCESS;
		}
		else
		{
			JPT_ERROR("Failed to load vkCreateDebugUtilsMessengerEXT function!");
			return false;
		}
	}

	void DebugMessenger::Shutdown()
	{
		const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
		const VkInstance vkInstance = pVulkanRenderer->GetVkInstance();

		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkInstance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(vkInstance, m_debugMessenger, nullptr);
		}
	}

	VkDebugUtilsMessengerCreateInfoEXT DebugMessenger::MakeCreateInfo()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};

		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		createInfo.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		createInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = GetApplication();

		return createInfo;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessenger::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		[[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		[[maybe_unused]] void* pUserData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			[[fallthrough]];
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			JPT_WARN(pCallbackData->pMessage);
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			JPT_ERROR(pCallbackData->pMessage);
			break;

		default:
			break;
		}

		return VK_FALSE;
	}
}