// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.CommandBuffer;

import jpt.Graphics.Constants;

import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;

import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	/** A command buffer is a buffer that holds commands that are sent to the GPU */
	class CommandBuffer
	{
	private:
		VkCommandBuffer m_commandBuffer;

	public:
		bool Init(const LogicalDevice& logicalDevice, const CommandPool& commandPool);
		void Shutdown(const LogicalDevice& logicalDevice, const CommandPool& commandPool);

	public:
		VkCommandBuffer GetHandle() const { return m_commandBuffer; }
		VkCommandBuffer* GetHandlePtr() { return &m_commandBuffer; }
	};

	bool CommandBuffer::Init(const LogicalDevice& logicalDevice, const CommandPool& commandPool)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool.GetHandle();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = kMaxFramesInFlight;

		if (const VkResult result = vkAllocateCommandBuffers(logicalDevice.GetHandle(), &allocInfo, &m_commandBuffer); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to allocate command buffers: %d", result);
			return false;
		}

		return true;
	}

	void CommandBuffer::Shutdown(const LogicalDevice& logicalDevice, const CommandPool& commandPool)
	{
		vkFreeCommandBuffers(logicalDevice.GetHandle(), commandPool.GetHandle(), kMaxFramesInFlight, &m_commandBuffer);
	}
}