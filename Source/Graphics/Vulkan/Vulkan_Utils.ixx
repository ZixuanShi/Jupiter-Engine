// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Utils;

import jpt.Vulkan.CommandPool;
import jpt.TVector3;

export namespace jpt::Vulkan
{
	void GenerateSierpinski(size_t depth, Vec3f right, Vec3f left, Vec3f top);

	VkCommandBuffer BeginSingleTimeCommand(const CommandPool& commandPool);

	void EndSingleTimeCommand(const CommandPool& commandPool, VkCommandBuffer commandBuffer);

	void CreateImage(uint32 width, uint32 height, uint32 mipLevels,
		VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkImage& image, VkDeviceMemory& imageMemory);

	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32 mipLevels);

	void TransitionImageLayout(const CommandPool& commandPool,
		VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, uint32 mipLevels);

	void CopyBufferToImage(const CommandPool& commandPool,
		VkBuffer buffer, VkImage image, uint32 width, uint32 height);

	bool HasStencilComponent(VkFormat format);

	void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32 texWidth, int32 texHeight, uint32 mipLevels);
}