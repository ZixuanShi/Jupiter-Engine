// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.SwapChain.SupportDetails;

import jpt.Vulkan.Constants;

import jpt.Application;
import jpt.Renderer;

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

        bool supportsMailbox = false;

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
        // VSync off
        if (!GetApplication()->GetRenderer()->GetSettings().VSyncOn)
        {
            JPT_ASSERT(supportsMailbox, "Current GPU doesn't support VK_PRESENT_MODE_MAILBOX_KHR, can't turn off VSync");
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }

        // VSync on
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
        return !formats.IsEmpty();
    }
}