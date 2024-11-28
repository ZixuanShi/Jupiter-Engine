// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.DebugMessenger;

import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	/** Vulkan Debug messenger */
	class VulkanDebugMessenger
	{
	private:
		VkDebugUtilsMessengerEXT m_debugMessenger;

	public:
		bool Init(VkInstance instance);
		void Shutdown(VkInstance instance);

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType, 
                                                            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
                                                            void* pUserData);

		VkDebugUtilsMessengerCreateInfoEXT GetCreateInfo() const;

	private:
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pCallback);

		void DestroyDebugMessenger(VkInstance instance,
			VkDebugUtilsMessengerEXT callback,
			const VkAllocationCallbacks* pAllocator);
	};

	bool VulkanDebugMessenger::Init(VkInstance instance)
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo = GetCreateInfo();

		if (const VkResult result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &m_debugMessenger); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to set up debug messenger! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	void VulkanDebugMessenger::Shutdown(VkInstance instance)
	{
		DestroyDebugMessenger(instance, m_debugMessenger, nullptr);
	}

	VkDebugUtilsMessengerCreateInfoEXT VulkanDebugMessenger::GetCreateInfo() const
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr; // Optional

		return createInfo;
	}

	VkResult VulkanDebugMessenger::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VulkanDebugMessenger::DestroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, callback, pAllocator);
		}
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugMessenger::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
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
			JPT_WARN("Validation layer: %s", pCallbackData->pMessage);
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			JPT_ERROR("Validation layer: %s", pCallbackData->pMessage);
			break;

		default:
			break;
		}

		return VK_FALSE;
	}
}