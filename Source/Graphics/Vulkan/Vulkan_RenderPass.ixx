// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.RenderPass;

import jpt.Vulkan.LogicalDevice;

export namespace jpt::Vulkan
{
	/** Describes the attachments that will be used during the rendering process
		We need to specify how many color and depth buffers there will be, 
		how many samples to use for each of them and how their contents should be handled throughout the rendering operations.*/
	class RenderPass
	{
	private:
		VkRenderPass m_renderPass = VK_NULL_HANDLE;

	public:
		bool Init(const LogicalDevice& logicalDevice, VkFormat imageFormat);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkRenderPass GetHandle() const { return m_renderPass; }
	};

	bool RenderPass::Init(const LogicalDevice& logicalDevice, VkFormat imageFormat)
	{
		// Describes the attachments that will be used during the rendering process
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = imageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// Subpasses are subsequent rendering operations that depend on the contents of framebuffers in previous passes
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		if (const VkResult result = vkCreateRenderPass(logicalDevice.GetHandle(), &renderPassInfo, nullptr, &m_renderPass); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create render pass: %d", result);
			return false;
		}

		return true;
	}

	void RenderPass::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyRenderPass(logicalDevice.GetHandle(), m_renderPass, nullptr);
	}
}