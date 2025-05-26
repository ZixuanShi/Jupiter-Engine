// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.SwapChain.SupportDetails;

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

            bool supportsMailbox = false;

        public:
            VkSurfaceFormatKHR ChooseSwapSurfaceFormat() const;
            VkPresentModeKHR ChooseSwapPresentMode() const;
            VkExtent2D ChooseSwapExtent(Window* pWindow) const;

            bool IsValid() const;
        };
    }
}