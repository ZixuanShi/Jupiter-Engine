// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

export module jpt.Vulkan_Image;

import jpt.TypeDefs;
import jpt.FilePath;

export namespace jpt::Vulkan
{
    /** Raw data container */
    class Image_Vulkan
    {
    private:
        VkImage m_handle = VK_NULL_HANDLE;
        VkDeviceMemory m_memory = VK_NULL_HANDLE;
        
        VkFormat m_format = VK_FORMAT_R8G8B8A8_SRGB;
        int32 m_width = 0;
        int32 m_height = 0;
        uint32 m_mipLevels = 0;

    public:
        bool Load(const File::Path& fullPath);
        void Shutdown();

    public:
        VkImage GetHandle() const { return m_handle; }
        VkDeviceMemory GetMemory() const { return m_memory; }
        VkFormat GetFormat() const { return m_format; }
        int32 GetWidth() const { return m_width; }
        int32 GetHeight() const { return m_height; }
        uint32 GetMipLevels() const { return m_mipLevels; }
    };
}