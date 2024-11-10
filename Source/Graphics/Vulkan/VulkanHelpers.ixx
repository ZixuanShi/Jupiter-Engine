// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Helpers;

import jpt.QueueFamilyIndices;

import jpt.DynamicArray;

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

	bool CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32 extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		DynamicArray<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.Buffer());

		return true;
	}
}