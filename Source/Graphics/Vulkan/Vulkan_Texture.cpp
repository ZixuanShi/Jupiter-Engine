// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <vulkan/vulkan.h>

#include <cmath>

module jpt.Vulkan.Texture;

import jpt.Application;
import jpt.Renderer;
import jpt.Renderer_Vulkan;

import jpt.Vulkan.Constants;
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

	void Texture_Vulkan::Shutdown()
	{
		Renderer_Vulkan* pRendererVulkan = GetApplication()->GetRenderer<Renderer_Vulkan>();
		const LogicalDevice& logicalDevice = pRendererVulkan->GetLogicalDevice();

		m_image.Shutdown();

		vkDestroyImageView(logicalDevice.GetHandle(), m_imageView, nullptr);
		m_imageView   = VK_NULL_HANDLE;
	}

	bool Texture_Vulkan::CreateImageView()
	{
		m_imageView = jpt::Vulkan::CreateImageView(m_image.GetHandle(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, m_image.GetMipLevels());

		return true;
	}
}