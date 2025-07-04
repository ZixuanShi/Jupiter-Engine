// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

export module jpt.Vulkan_ValidationLayers;

import jpt.Application;
import jpt.StringHelpers;
import jpt.DynamicArray;
import jpt.TypeDefs;

import jpt.Framework;

export namespace jpt::Vulkan
{
    const DynamicArray<const char*> g_validationLayers = 
    {
        "VK_LAYER_KHRONOS_validation"
    };

    bool CheckValidationLayerSupport()
    {
        uint32 layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        DynamicArray<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.Buffer());

        for (const char* layerName : g_validationLayers)
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