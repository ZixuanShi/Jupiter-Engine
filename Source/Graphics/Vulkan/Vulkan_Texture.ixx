// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Texture;

import jpt.Texture;

import jpt.Vulkan.Utils;
import jpt.Vulkan.Buffer;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;

import jpt.File.Path;
import jpt.File.IO;

export namespace jpt::Vulkan
{
	class Texture_Vulkan final : public Texture
	{
	private:
		VkImage m_image = VK_NULL_HANDLE;
		VkDeviceMemory m_imageMemory = VK_NULL_HANDLE;
		VkImageView m_imageView = VK_NULL_HANDLE;

		PhysicalDevice* m_pPhysicalDevice;
		LogicalDevice* m_pLogicalDevice;
		CommandPool* m_pCommandPool;

	public:
		bool Init(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool);
		virtual void Shutdown() override;

		virtual bool Load(const File::Path& fullPath) override;

	public:
		VkImageView GetImageView() const { return m_imageView; }

	private:
		bool CreateImage(const File::Path& fullPath);
		bool CreateImageView();
	};
}