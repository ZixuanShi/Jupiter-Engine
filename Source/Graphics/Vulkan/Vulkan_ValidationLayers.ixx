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

#if !IS_RELEASE
		extensions.EmplaceBack(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
		return extensions;
	}

	/** @return		How suitable the device fit Jupiter's vulkan renderer as score. 0 means not eligible at all */
	uint32 GetDeviceScore(VkPhysicalDevice device)
	{
		uint32 score = 0;

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		if (!deviceFeatures.geometryShader)
		{
			return 0;
		}

		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			score += 1000;
		}

		return score;
	}
}