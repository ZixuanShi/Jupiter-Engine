// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.CommandBuffer;

export namespace jpt::Vulkan
{
	/** Command buffers are objects used to record commands which can be submitted to a device queue for execution */
	class CommandBuffer
	{
	private:
		VkCommandBuffer m_commandBuffer;

	public:
	};
}