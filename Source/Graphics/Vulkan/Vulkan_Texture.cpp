// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <vulkan/vulkan.h>

#include <cmath>

module jpt.Vulkan_Texture;

import jpt.Vulkan_Constants;
import jpt.Vulkan_Buffer;
import jpt.Vulkan_CommandPool;
import jpt.Vulkan_LogicalDevice;
import jpt.Vulkan_PhysicalDevice;
import jpt.Vulkan_Utils;

import jpt.Math;

import jpt.FileIO;

namespace jpt::Vulkan
{
    bool Texture_Vulkan::Load(const File::Path& fullPath)
    {
        if (m_imageView != VK_NULL_HANDLE)
        {
            Unload();
        }

        JPT_ASSERT(File::Exists(fullPath));

        if (!m_image.Load(fullPath))
        {
            return false;
        }

        if (!CreateImageView())
        {
            return false;
        }

        return true;
    }

    void Texture_Vulkan::Unload()
    {
        m_image.Terminate();

        LogicalDevice::Get().WaitIdle();
        vkDestroyImageView(LogicalDevice::GetVkDevice(), m_imageView, nullptr);
        m_imageView   = VK_NULL_HANDLE;
    }

    bool Texture_Vulkan::CreateImageView()
    {
        m_imageView = jpt::Vulkan::CreateImageView(m_image.GetHandle(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, m_image.GetMipLevels());

        return m_imageView != VK_NULL_HANDLE;
    }
}