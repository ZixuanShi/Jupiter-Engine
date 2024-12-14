// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.CommandPool;

import jpt.Vulkan.LogicalDevice;

import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	/** A command pool is a memory pool that holds command buffers */
	class CommandPool
	{
	private:
		VkCommandPool m_commandPool = VK_NULL_HANDLE;

	public:
		bool Init(const LogicalDevice& logicalDevice, uint32 graphicsFamilyIndex);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkCommandPool GetHandle() const { return m_commandPool; }
	};

	bool CommandPool::Init(const LogicalDevice& logicalDevice, uint32 graphicsFamilyIndex)
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = graphicsFamilyIndex;

		if (const VkResult result = vkCreateCommandPool(logicalDevice.GetHandle(), &poolInfo, nullptr, &m_commandPool); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create command pool: %d", result);
			return false;
		}

		return true;
	}

	void CommandPool::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyCommandPool(logicalDevice.GetHandle(), m_commandPool, nullptr);
		m_commandPool = VK_NULL_HANDLE;
	}
}