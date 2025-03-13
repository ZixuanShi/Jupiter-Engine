// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.SwapChain.SupportDetails;

import jpt.Vulkan.Constants;

import jpt.Window;
import jpt.Constants;
import jpt.DynamicArray;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Vector2;

export namespace jpt::Vulkan
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        DynamicArray<VkSurfaceFormatKHR> formats;
        DynamicArray<VkPresentModeKHR> presentModes;

    public:
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat() const;
        VkPresentModeKHR ChooseSwapPresentMode() const;
        VkExtent2D ChooseSwapExtent(Window* pWindow) const;

        bool IsValid() const;
    };

    VkSurfaceFormatKHR SwapChainSupportDetails::ChooseSwapSurfaceFormat() const
    {
        for (const VkSurfaceFormatKHR& format : formats)
        {
            if (format.format == kFormat && 
                format.colorSpace == kColorSpace)
            {
                return format;
            }
        }

        return formats[0];
    }

    VkPresentModeKHR SwapChainSupportDetails::ChooseSwapPresentMode() const
    {
        for (const VkPresentModeKHR& presentMode : presentModes)
        {
            if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return presentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChainSupportDetails::ChooseSwapExtent(Window* pWindow) const
    {
        if (capabilities.currentExtent.width != Constants<uint32>::kMax)
        {
            return capabilities.currentExtent;
        }
        else
        {
            Vec2i frameSize = pWindow->GetFrameSize();

            VkExtent2D actualExtent = 
            {
                static_cast<uint32>(frameSize.x),
                static_cast<uint32>(frameSize.y)
            };

            actualExtent.width = Clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = Clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    bool jpt::Vulkan::SwapChainSupportDetails::IsValid() const
    {
        return !formats.IsEmpty() && !presentModes.IsEmpty();
    }
}