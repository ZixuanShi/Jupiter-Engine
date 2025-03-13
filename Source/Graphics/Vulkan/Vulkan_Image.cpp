// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <vulkan/vulkan.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

module jpt.Vulkan.Image;

import jpt.Application;
import jpt.Renderer;
import jpt.Renderer_Vulkan;

import jpt.Vulkan.Buffer;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.Utils;

import jpt.Math;

import jpt.File.IO;

namespace jpt::Vulkan
{
    bool Image_Vulkan::Load(const File::Path& fullPath)
    {
        Renderer_Vulkan* pRendererVulkan = GetVkRenderer();
        const CommandPool& memTransferCommandPool = pRendererVulkan->GetMemoryTransferCommandPool();

        int32 texChannels = 0;
        unsigned char* pixels = stbi_load(ToString(fullPath).ConstBuffer(), &m_width, &m_height, &texChannels, STBI_rgb_alpha);
        JPT_ASSERT(pixels, "Failed to load texture image");
        m_mipLevels = static_cast<uint32>(Floor(Log2(Max(m_width, m_height)))) + 1;

        const VkDeviceSize imageSize = m_width * m_height * 4;
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = imageSize;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        const VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        Buffer stagingBuffer;
        stagingBuffer.Create(bufferInfo, properties);
        stagingBuffer.MapMemory(pixels, imageSize);

        stbi_image_free(pixels);

        jpt::Vulkan::CreateImage(m_width, m_height, m_mipLevels,
            VK_SAMPLE_COUNT_1_BIT, m_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_handle, m_memory);

        TransitionImageLayout(memTransferCommandPool,
            m_handle, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipLevels);
        {
            CopyBufferToImage(memTransferCommandPool,
                stagingBuffer.GetHandle(), m_handle, static_cast<uint32>(m_width), static_cast<uint32>(m_height));
        }

        stagingBuffer.Shutdown();

        GenerateMipmaps(m_handle, m_format, m_width, m_height, m_mipLevels);

        return true;
    }

    void Image_Vulkan::Shutdown()
    {
        LogicalDevice::Get().WaitIdle();

        vkDestroyImage(LogicalDevice::GetVkDevice(), m_handle, nullptr);
        vkFreeMemory(LogicalDevice::GetVkDevice(), m_memory, nullptr);

        m_handle = VK_NULL_HANDLE;
        m_memory = VK_NULL_HANDLE;
    }
}