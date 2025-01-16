// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <vulkan/vulkan.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cmath>

module jpt.Vulkan.Texture;

import jpt.Application;
import jpt.Renderer;
import jpt.Renderer_Vulkan;

import jpt.Vulkan.Buffer;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.PhysicalDevice;
import jpt.Vulkan.Utils;

import jpt.Math;

import jpt.File.IO;

namespace jpt::Vulkan
{
	bool Texture_Vulkan::Load(const File::Path& fullPath)
	{
		if (m_imageView != VK_NULL_HANDLE)
		{
			Shutdown();
		}

		JPT_ASSERT(File::Exists(fullPath));

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

	void Texture_Vulkan::Shutdown()
	{
		Renderer_Vulkan* pRendererVulkan = GetApplication()->GetRenderer<Renderer_Vulkan>();
		const LogicalDevice& logicalDevice = pRendererVulkan->GetLogicalDevice();

		vkDestroyImageView(logicalDevice.GetHandle(), m_imageView, nullptr);
		vkDestroyImage(logicalDevice.GetHandle(), m_image, nullptr);
		vkFreeMemory(logicalDevice.GetHandle(), m_imageMemory, nullptr);

		m_imageView   = VK_NULL_HANDLE;
		m_image       = VK_NULL_HANDLE;
		m_imageMemory = VK_NULL_HANDLE;
	}

	bool Texture_Vulkan::CreateImage(const File::Path& fullPath)
	{
		Renderer_Vulkan* pRendererVulkan = GetApplication()->GetRenderer<Renderer_Vulkan>();
		const CommandPool& memTransferCommandPool = pRendererVulkan->GetMemoryTransferCommandPool();

		int32 texWidth, texHeight, texChannels;
		unsigned char* pixels = stbi_load(fullPath.ToCString().ConstBuffer(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		JPT_ASSERT(pixels, "Failed to load texture image");
		m_mipLevels = static_cast<uint32>(Floor(Log2(Max(texWidth, texHeight)))) + 1;

		const VkDeviceSize imageSize = texWidth * texHeight * 4;
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = imageSize;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		Buffer stagingBuffer;
		stagingBuffer.Create(bufferInfo, properties);
		stagingBuffer.MapMemory(pixels, imageSize);

		stbi_image_free(pixels);

		jpt::Vulkan::CreateImage(texWidth, texHeight, m_mipLevels,
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_image, m_imageMemory);

		TransitionImageLayout(memTransferCommandPool,
			m_image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mipLevels);
		{
			CopyBufferToImage(memTransferCommandPool,
				stagingBuffer.GetHandle(), m_image, static_cast<uint32>(texWidth), static_cast<uint32>(texHeight));
		}

		stagingBuffer.Shutdown();

		GenerateMipmaps(m_image, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, m_mipLevels);

		return true;
	}

	bool Texture_Vulkan::CreateImageView()
	{
		m_imageView = jpt::Vulkan::CreateImageView(m_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels);

		return true;
	}
}