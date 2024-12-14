// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.CommandBuffers;

import jpt.Graphics.Constants;

import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;

import jpt.StaticArray;
import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	/** A command buffer is a buffer that holds commands that are sent to the GPU */
	class CommandBuffers
	{
	private:
		StaticArray<VkCommandBuffer, kMaxFramesInFlight> m_commandBuffers;

	public:
		bool Init(const LogicalDevice& logicalDevice, const CommandPool& commandPool);
		void Shutdown(const LogicalDevice& logicalDevice, const CommandPool& commandPool);

	public:
		VkCommandBuffer GetHandle(uint32 index) const { return m_commandBuffers[index]; }
		VkCommandBuffer* GetHandlePtr(uint32 index) { return &m_commandBuffers[index]; }
	};

	bool CommandBuffers::Init(const LogicalDevice& logicalDevice, const CommandPool& commandPool)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool.GetHandle();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = kMaxFramesInFlight;

		if (const VkResult result = vkAllocateCommandBuffers(logicalDevice.GetHandle(), &allocInfo, m_commandBuffers.Buffer()); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to allocate command buffers: %d", result);
			return false;
		}

		return true;
	}

	void CommandBuffers::Shutdown(const LogicalDevice& logicalDevice, const CommandPool& commandPool)
	{
		vkFreeCommandBuffers(logicalDevice.GetHandle(), commandPool.GetHandle(), kMaxFramesInFlight, m_commandBuffers.Buffer());
	}
}