// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.CommandBuffers;

import jpt.Graphics.Constants;

import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.RenderPass;
import jpt.Vulkan.SwapChain;
import jpt.Vulkan.GraphicsPipeline;

import jpt.StaticArray;
import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	/** A command buffer is a buffer that holds commands that are sent to the GPU */
	class CommandBuffer
	{
	private:
		//StaticArray<VkCommandBuffer, kMaxFramesInFlight> m_commandBuffers;
		VkCommandBuffer m_commandBuffers;

	public:
		bool Init(const LogicalDevice& logicalDevice, const CommandPool& commandPool);
		void Shutdown(const LogicalDevice& logicalDevice, const CommandPool& commandPool);

		void Record(const LogicalDevice& logicalDevice, const CommandPool& commandPool, uint32 imageIndex, 
			const RenderPass& renderPass, const SwapChain& swapChain, const GraphicsPipeline& graphicsPipeline);

	public:
	};

	bool CommandBuffer::Init(const LogicalDevice& logicalDevice, const CommandPool& commandPool)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool.GetHandle();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if (const VkResult result = vkAllocateCommandBuffers(logicalDevice.GetHandle(), &allocInfo, &m_commandBuffers); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to allocate command buffers: %d", result);
			return false;
		}

		return true;
	}

	void CommandBuffer::Shutdown(const LogicalDevice& logicalDevice, const CommandPool& commandPool)
	{
		vkFreeCommandBuffers(logicalDevice.GetHandle(), commandPool.GetHandle(), kMaxFramesInFlight, &m_commandBuffers);
	}

	void CommandBuffer::Record(const LogicalDevice& logicalDevice, const CommandPool& commandPool, uint32 imageIndex, 
		const RenderPass& renderPass, const SwapChain& swapChain, const GraphicsPipeline& graphicsPipeline)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (const VkResult result = vkBeginCommandBuffer(m_commandBuffers, &beginInfo); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to begin recording command buffer: %d", result);
			return;
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass.GetHandle();
		renderPassInfo.framebuffer = swapChain.GetFramebuffers()[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChain.GetExtent();

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		// Start recording commands
		vkCmdBeginRenderPass(m_commandBuffers, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		{
			vkCmdBindPipeline(m_commandBuffers, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetHandle());

			VkViewport viewport = {};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(swapChain.GetExtent().width);
			viewport.height = static_cast<float>(swapChain.GetExtent().height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(m_commandBuffers, 0, 1, &viewport);

			VkRect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent = swapChain.GetExtent();
			vkCmdSetScissor(m_commandBuffers, 0, 1, &scissor);

			vkCmdDraw(m_commandBuffers, 3, 1, 0, 0);
		}
		vkCmdEndRenderPass(m_commandBuffers);

		if (const VkResult result = vkEndCommandBuffer(m_commandBuffers); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to record command buffer: %d", result);
		}
	}
}