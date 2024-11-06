// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/App/Application.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Utils;

import jpt.DynamicArray;

import jpt.TypeDefs;
import jpt.Optional;

import jpt.Framework;

export namespace jpt
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

	struct QueueFamilyIndices
	{
		Optional<uint32> graphicsFamily;
	};

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		DynamicArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.Buffer());

		uint32 i = 0;
		for (const VkQueueFamilyProperties& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}

			if (indices.graphicsFamily)
			{
				break;
			}

			++i;
		}

		return indices;
	}

	/** @return		How suitable the device fit Jupiter's vulkan renderer as score. 0 means not eligible at all */
	uint32 GetDeviceScore(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = FindQueueFamilies(device);
		if (!indices.graphicsFamily)
		{
			return 0;
		}

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		if (!deviceFeatures.geometryShader)
		{
			return 0;
		}

		uint32 score = 0;

		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			score += 1000;
		}

		return score;
	}
}