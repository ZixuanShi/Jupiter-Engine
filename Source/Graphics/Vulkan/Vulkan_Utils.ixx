// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Utils;

import jpt.Vulkan.Vertex;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.LogicalDevice;

import jpt.Vector2;
import jpt.Vector3;

export namespace jpt::Vulkan
{
	void GenerateSierpinski(size_t depth, Vec2f right, Vec2f left, Vec2f top)
	{
		if (depth <= 0)
		{
			static const Vec3f klocTriangleColor = { 0.95f, 0.05f, 0.05f };
			const size_t baseIndex = g_vertices.Count();

			g_vertices.EmplaceBack(Vertex(right, klocTriangleColor));
			g_vertices.EmplaceBack(Vertex(left, klocTriangleColor));
			g_vertices.EmplaceBack(Vertex(top, klocTriangleColor));

			g_indices.EmplaceBack(static_cast<uint16>(baseIndex));     // right
			g_indices.EmplaceBack(static_cast<uint16>(baseIndex + 1)); // left  
			g_indices.EmplaceBack(static_cast<uint16>(baseIndex + 2)); // top

			return;
		}

		const Vec2f rightLeft = (right + left) * 0.5f;
		const Vec2f rightTop = (right + top) * 0.5f;
		const Vec2f leftTop = (left + top) * 0.5f;

		GenerateSierpinski(depth - 1, right, rightLeft, rightTop);
		GenerateSierpinski(depth - 1, rightLeft, left, leftTop);
		GenerateSierpinski(depth - 1, rightTop, leftTop, top);
	}

	VkCommandBuffer BeginSingleTimeCommand(const LogicalDevice& logicalDevice, const CommandPool& commandPool)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool.GetHandle();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(logicalDevice.GetHandle(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void EndSingleTimeCommand(const LogicalDevice& logicalDevice, const CommandPool& commandPool, VkCommandBuffer commandBuffer)
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(logicalDevice.GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(logicalDevice.GetGraphicsQueue());

		vkFreeCommandBuffers(logicalDevice.GetHandle(), commandPool.GetHandle(), 1, &commandBuffer);
	}
}