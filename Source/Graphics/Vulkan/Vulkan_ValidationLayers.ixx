// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

export module jpt.Vulkan.ValidationLayers;

import jpt.Constants;
import jpt.TypeDefs;

import jpt.DynamicArray;

import jpt.String.Helpers;

export namespace jpt
{
	const DynamicArray<const char*> validationLayers = 
	{
		"VK_LAYER_KHRONOS_validation"
	};

#if IS_DEBUG
	constexpr bool kEnableValidationLayers = true;
#else
	constexpr bool kEnableValidationLayers = false;
#endif

	bool CheckValidationLayerSupport()
	{
		uint32 layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		DynamicArray<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.Buffer());

		for (const char* layerName : validationLayers)
		{
			bool layerFound = false;

			for (const VkLayerProperties& layerProperties : availableLayers)
			{
				if (StrCmp(layerName, layerProperties.layerName) == npos)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}

		return true;
	}

	DynamicArray<const char*> GetRequiredExtensions()
	{
		uint32 glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		DynamicArray<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if constexpr (kEnableValidationLayers)
		{
			extensions.EmplaceBack(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}
}