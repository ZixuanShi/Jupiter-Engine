// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Texture;

import jpt.Texture;
import jpt.TypeDefs;

import jpt.Vulkan.Image;

export namespace jpt::Vulkan
{
	/** Image and Sampling/Access information */
	class Texture_Vulkan final : public Texture
	{
	private:
		Image_Vulkan m_image;
		VkImageView m_imageView = VK_NULL_HANDLE;

	public:
		virtual void Shutdown() override;

		virtual bool Load(const File::Path& fullPath) override;

	public:
		VkImageView GetImageView() const { return m_imageView; }

	private:
		bool CreateImageView();
	};
}