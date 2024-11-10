// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Helpers;

import jpt.Vulkan.QueueFamilyIndices;

import jpt.DynamicArray;
import jpt.HashSet;

import jpt.String;
import jpt.TypeDefs;
import jpt.Optional;

import jpt.Framework;

export namespace jpt::Vulkan
{
	DynamicArray<const char*> GetRequiredExtensions()
	{
		uint32 glfwExtensionCount = 0;
		const char** glfwExtensions = GetApplication()->GetFramework()->GetRequiredExtensions(glfwExtensionCount);

		DynamicArray<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#if !IS_RELEASE
		extensions.EmplaceBack(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
		return extensions;
	}

	const DynamicArray<const char*> deviceExtensions = 
	{ 
		VK_KHR_SWAPCHAIN_EXTENSION_NAME 
	};

	bool CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32 extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		DynamicArray<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.Buffer());

		HashSet<String> requiredExtensions;
		for (const char* extension : deviceExtensions)
		{
			requiredExtensions.Add(extension);
		}

		for (const VkExtensionProperties& extension : availableExtensions)
		{
			requiredExtensions.Erase(extension.extensionName);
		}

		if (!requiredExtensions.IsEmpty())
		{
			for (const String& extension : requiredExtensions)
			{
				JPT_ERROR("Missing required extension: %s", extension.ConstBuffer());
			}
		}

		return requiredExtensions.IsEmpty();
	}
}