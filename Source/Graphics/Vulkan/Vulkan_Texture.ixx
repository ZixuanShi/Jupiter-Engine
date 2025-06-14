// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

export module jpt.Vulkan_Texture;

import jpt.Texture;
import jpt.TypeDefs;

import jpt.Vulkan_Image;

export namespace jpt::Vulkan
{
    /** Image and Sampling/Access information */
    class Texture_Vulkan final : public Texture
    {
    private:
        Image_Vulkan m_image;
        VkImageView m_imageView = VK_NULL_HANDLE;

    public:
        virtual bool Load(const File::Path& fullPath) override;
        virtual void Unload() override;

    public:
        const Image_Vulkan& GetImage() const { return m_image; }
        VkImageView GetImageView() const { return m_imageView; }

    private:
        bool CreateImageView();
    };
}