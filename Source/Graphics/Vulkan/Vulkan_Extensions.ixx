// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

export module jpt.Vulkan_Extensions;

import jpt.Application;
import jpt.DynamicArray;
import jpt.Framework;
import jpt.TypeDefs;

export namespace jpt::Vulkan
{
    DynamicArray<const char*> GetRequiredExtensions()
    {
        uint32 extensionCount = 0;
        const char** extensions = GetApplication()->GetFramework()->GetRequiredExtensions(extensionCount);

        DynamicArray<const char*> result(extensions, extensions + extensionCount);

#if !IS_CONFIG_RELEASE
        result.EmplaceBack(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        return result;
    }

    const DynamicArray<const char*> g_deviceExtensions = 
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
}