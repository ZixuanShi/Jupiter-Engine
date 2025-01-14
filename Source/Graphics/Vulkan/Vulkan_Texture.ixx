// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Texture;

import jpt.Texture;
import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	class Texture_Vulkan final : public Texture
	{
	private:
		VkImage m_image = VK_NULL_HANDLE;
		VkDeviceMemory m_imageMemory = VK_NULL_HANDLE;
		VkImageView m_imageView = VK_NULL_HANDLE;
		uint32 m_mipLevels = 0;

	public:
		virtual void Shutdown() override;

		virtual bool Load(const File::Path& fullPath) override;

	public:
		VkImageView GetImageView() const { return m_imageView; }

	private:
		bool CreateImage(const File::Path& fullPath);
		bool CreateImageView();
	};
}