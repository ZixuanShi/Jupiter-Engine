// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.CommandBuffers;

import jpt.Graphics.Constants;

import jpt.Vulkan.Constants;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.CommandPool;
import jpt.Vulkan.RenderPass;
import jpt.Vulkan.SwapChain;
import jpt.Vulkan.PipelineLayout;
import jpt.Vulkan.Pipeline;
import jpt.Vulkan.Data;
import jpt.Vulkan.VertexBuffer;
import jpt.Vulkan.IndexBuffer;

import jpt.StaticArray;

export namespace jpt::Vulkan
{
	/** Command buffers are objects used to record commands which can be submitted to a device queue for execution */
	class CommandBuffers
	{
	private:
		StaticArray<VkCommandBuffer, kMaxFramesInFlight> m_commandBuffers;

	public:
		bool Init(const LogicalDevice& logicalDevice, const CommandPool& commandPool);
		void Shutdown(const LogicalDevice& logicalDevice, const CommandPool& commandPool);

	public:
		void Reset(size_t index, VkCommandBufferResetFlags flags = 0) const;
		void Record(size_t index, uint32 imageIndex, const RenderPass& renderPass, const SwapChain& swapChain, const Pipeline& graphicsPipeline, 
			const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, 
			const PipelineLayout& pipelineLayout, const StaticArray<VkDescriptorSet, kMaxFramesInFlight>& descriptorSets) const;

	public:
		const VkCommandBuffer* GetBufferPtr(size_t index) const { return &m_commandBuffers[index]; }
		VkCommandBuffer GetBuffer(size_t index) const { return m_commandBuffers[index]; }
	};

	bool CommandBuffers::Init(const LogicalDevice& logicalDevice, const CommandPool& commandPool)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool.Get();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32>(m_commandBuffers.Count());

		if (const VkResult result = vkAllocateCommandBuffers(logicalDevice.Get(), &allocInfo, m_commandBuffers.Buffer()); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to allocate command buffers! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		return true;
	}

	void CommandBuffers::Shutdown(const LogicalDevice& logicalDevice, const CommandPool& commandPool)
	{
		vkFreeCommandBuffers(logicalDevice.Get(), commandPool.Get(),
			static_cast<uint32>(m_commandBuffers.Count()),
			m_commandBuffers.Buffer());

		for (VkCommandBuffer& commandBuffer : m_commandBuffers)
		{
			commandBuffer = VK_NULL_HANDLE;
		}
	}

	void CommandBuffers::Reset(size_t index, VkCommandBufferResetFlags flags) const
	{
		vkResetCommandBuffer(m_commandBuffers[index], flags);
	}

	void CommandBuffers::Record(size_t currentFrame, uint32 imageIndex, const RenderPass& renderPass, const SwapChain& swapChain, const Pipeline& graphicsPipeline, 
		const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, 
		const PipelineLayout& pipelineLayout, const StaticArray<VkDescriptorSet, kMaxFramesInFlight>& descriptorSets) const
	{
		VkCommandBuffer commandBuffer = m_commandBuffers[currentFrame];

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (const VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to begin recording command buffer! VkResult: %i", static_cast<uint32>(result));
			return;
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass.Get();
		renderPassInfo.framebuffer = swapChain.GetFramebuffers()[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChain.GetExtent();

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		{
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.Get());

			// Set dynamic viewport
			VkViewport viewport = {};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(swapChain.GetExtent().width);
			viewport.height = static_cast<float>(swapChain.GetExtent().height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

			// Set dynamic scissor
			VkRect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent = swapChain.GetExtent();
			vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

			// Bind vertex buffer
			VkBuffer vertexBuffers[] = { vertexBuffer.Get() };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffer, indexBuffer.Get(), 0, VK_INDEX_TYPE_UINT32);

			// Draw
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout.Get(), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
			vkCmdDrawIndexed(commandBuffer, static_cast<uint32>(indices.Count()), 1, 0, 0, 0);
		}
		vkCmdEndRenderPass(commandBuffer);

		if (const VkResult result = vkEndCommandBuffer(commandBuffer); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to record command buffer! VkResult: %i", static_cast<uint32>(result));
		}
	}
}