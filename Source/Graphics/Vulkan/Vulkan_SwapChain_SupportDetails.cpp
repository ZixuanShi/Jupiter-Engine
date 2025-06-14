// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

module jpt.Vulkan_SwapChain_SupportDetails;

import jpt.Vulkan_Constants;

import jpt.Renderer;
import jpt.Graphics_Settings;
import jpt.Graphics_Enums;

import jpt.Window;
import jpt.Constants;
import jpt.Math;
import jpt.TypeDefs;
import jpt.Vector2;

namespace jpt::Vulkan
{
    VkSurfaceFormatKHR SwapChainSupportDetails::GetSurfaceFormat() const
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

    VkPresentModeKHR SwapChainSupportDetails::GetPresentMode() const
    {
        GraphicsSettings& graphicsSettings = GetGraphicsSettings();
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

        if (vsyncMode != VSyncMode::On)
        {
            JPT_ERROR("VSync mode \"%i\" is not supported on current machine. Changing to VSyncMode::On now", vsyncMode);
            graphicsSettings.SetVSyncMode(VSyncMode::On);
        }

        // Always supported
        return VK_PRESENT_MODE_FIFO_KHR; 
    }

    VkExtent2D SwapChainSupportDetails::GetExtent(Window* pWindow) const
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

            actualExtent.width  = Clamp(actualExtent.width,  capabilities.minImageExtent.width,  capabilities.maxImageExtent.width);
            actualExtent.height = Clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    bool jpt::Vulkan::SwapChainSupportDetails::IsValid() const
    {
        return !formats.IsEmpty();
    }
}