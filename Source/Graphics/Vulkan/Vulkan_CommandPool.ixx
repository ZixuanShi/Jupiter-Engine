// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.CommandPool;

import jpt.Vulkan.LogicalDevice;

import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	/** Memory Management
		- Command pools allocate memory for command buffers in bulk
		- They provide efficient reuse of memory resources
		- When you destroy a command pool, all command buffers allocated from it are automatically freed

		Threading Behavior
		- Command pools are not thread-safe internally
		- Each thread should have its own command pool
		- This design allows for efficient multi-threaded command recording */
	class CommandPool
	{
	private:
		VkCommandPool m_commandPool = VK_NULL_HANDLE;

	public:
		bool Init(const LogicalDevice& logicalDevice, uint32 queueFamilyIndex);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkCommandPool Get() const { return m_commandPool; }
	};

	bool CommandPool::Init(const LogicalDevice& logicalDevice, uint32 queueFamilyIndex)
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndex;

		if (const VkResult result = vkCreateCommandPool(logicalDevice.Get(), &poolInfo, nullptr, &m_commandPool); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create command pool! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	void CommandPool::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyCommandPool(logicalDevice.Get(), m_commandPool, nullptr);
		m_commandPool = VK_NULL_HANDLE;
	}
}