// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Validation/Assert.h"

#include <vulkan/vulkan.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
		void Shutdown();

		bool Load(const File::Path& fullPath);

	public:
		VkImageView GetImageView() const { return m_imageView; }

	private:
		bool CreateImage(const File::Path& fullPath);
		bool CreateImageView();
	};

	bool Texture_Vulkan::Load(const File::Path& fullPath)
	{
		if (m_imageView != VK_NULL_HANDLE)
		{
			Shutdown();
		}

		JPT_ASSERT(File::Exists(fullPath));
		JPT_ASSERT(m_pPhysicalDevice && m_pLogicalDevice && m_pCommandPool);

		if (!CreateImage(fullPath))
		{
			return false;
		}

		if (!CreateImageView())
		{
			return false;
		}

		return true;
	}

	bool Texture_Vulkan::Init(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool)
	{
		m_pPhysicalDevice = &physicalDevice;
		m_pLogicalDevice = &logicalDevice;
		m_pCommandPool = &commandPool;

		return true;
	}

	void Texture_Vulkan::Shutdown()
	{
		vkDestroyImageView(m_pLogicalDevice->GetHandle(), m_imageView, nullptr);
		vkDestroyImage(m_pLogicalDevice->GetHandle(), m_image, nullptr);
		vkFreeMemory(m_pLogicalDevice->GetHandle(), m_imageMemory, nullptr);

		m_imageView = VK_NULL_HANDLE;
		m_image = VK_NULL_HANDLE;
		m_imageMemory = VK_NULL_HANDLE;
	}

	bool Texture_Vulkan::CreateImage(const File::Path& fullPath)
	{
		int32 texWidth, texHeight, texChannels;
		unsigned char* pixels = stbi_load(fullPath.ToCString().ConstBuffer(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		JPT_ASSERT(pixels, "Failed to load texture image");

		const VkDeviceSize imageSize = texWidth * texHeight * 4;
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = imageSize;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		Buffer stagingBuffer;
		stagingBuffer.Create(bufferInfo, properties, *m_pLogicalDevice, *m_pPhysicalDevice);
		stagingBuffer.MapMemory(*m_pLogicalDevice, pixels, imageSize);

		stbi_image_free(pixels);

		jpt::Vulkan::CreateImage(*m_pLogicalDevice, *m_pPhysicalDevice,
			texWidth, texHeight,
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_image, m_imageMemory);

		TransitionImageLayout(*m_pLogicalDevice, *m_pCommandPool,
			m_image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		{
			CopyBufferToImage(*m_pLogicalDevice, *m_pCommandPool,
				stagingBuffer.GetHandle(), m_image, static_cast<uint32>(texWidth), static_cast<uint32>(texHeight));
		}
		TransitionImageLayout(*m_pLogicalDevice, *m_pCommandPool,
			m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		stagingBuffer.Shutdown(*m_pLogicalDevice);

		return true;
	}
	
	bool Texture_Vulkan::CreateImageView()
	{
		m_imageView = jpt::Vulkan::CreateImageView(*m_pLogicalDevice, m_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
		
		return true;
	}
}