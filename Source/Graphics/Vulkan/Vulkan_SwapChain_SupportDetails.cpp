// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

module jpt.Vulkan.SwapChain.SupportDetails;

import jpt.Vulkan.Constants;

import jpt.Renderer;
import jpt.Graphics.Settings;
import jpt.Graphics.Enums;

import jpt.Window;
import jpt.Constants;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Vector2;

namespace jpt::Vulkan
{
    VkSurfaceFormatKHR SwapChainSupportDetails::GetSwapSurfaceFormat() const
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

    VkPresentModeKHR SwapChainSupportDetails::GetSwapPresentMode() const
    {
        const GraphicsSettings& graphicsSettings = GetGraphicsSettings();
        const VSyncMode vsyncMode = graphicsSettings.GetVSyncMode();

        if (vsyncMode == VSyncMode::Fast && supportsMailbox)
        {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
        else if (vsyncMode == VSyncMode::Adaptive && supportsFifoRelaxed)
        {
            return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
        }
        else if (vsyncMode == VSyncMode::Off && supportsImmediate)
        {
            return VK_PRESENT_MODE_IMMEDIATE_KHR;
        }

        JPT_ASSERT(vsyncMode == VSyncMode::On, "VSync mode \"%s\" is not supported on current machine", ToString(vsyncMode).ConstBuffer());
        return VK_PRESENT_MODE_FIFO_KHR; // Always supported
    }

    VkExtent2D SwapChainSupportDetails::GetSwapExtent(Window* pWindow) const
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