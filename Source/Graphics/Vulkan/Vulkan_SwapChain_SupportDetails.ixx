// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan_SwapChain_SupportDetails;

import jpt.DynamicArray;

export namespace jpt
{
    class Window;

    namespace Vulkan
    {
        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR capabilities;
            DynamicArray<VkSurfaceFormatKHR> formats;

            bool supportsMailbox     : 1 = false;
            bool supportsImmediate   : 1 = false;
            bool supportsFifoRelaxed : 1 = false;

        public:
            VkSurfaceFormatKHR GetSurfaceFormat()         const;
            VkPresentModeKHR   GetPresentMode()           const;
            VkExtent2D         GetExtent(Window* pWindow) const;

            bool IsValid() const;
        };
    }
}