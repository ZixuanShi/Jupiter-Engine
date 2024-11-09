// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

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
				if (AreStringsSame(layerName, layerProperties.layerName))
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
}