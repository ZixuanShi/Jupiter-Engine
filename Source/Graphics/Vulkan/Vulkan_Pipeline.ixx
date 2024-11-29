// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Pipeline;

import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.SwapChain;
import jpt.Vulkan.Shader;
import jpt.Vulkan.PipelineLayout;
import jpt.Vulkan.RenderPass;

export namespace jpt::Vulkan
{
	class Pipeline
	{
	private:
		VkPipeline m_graphicsPipeline;

	public:
		bool Init(const LogicalDevice& logicalDevice, const SwapChain& swapChain, const PipelineLayout& pipelineLayout, const RenderPass& renderPass);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkPipeline Get() const { return m_graphicsPipeline; }
	};

	bool Pipeline::Init(const LogicalDevice& logicalDevice, const SwapChain& swapChain, const PipelineLayout& pipelineLayout, const RenderPass& renderPass)
	{
		// Shaders
		Shader vertexShader;
		VkPipelineShaderStageCreateInfo vertexShaderInfo = vertexShader.GetStageInfo(VK_SHADER_STAGE_VERTEX_BIT, "_Baked/Jupiter_Common/Shaders/triangle.vs.spv", logicalDevice);

		Shader pixelShader;
		VkPipelineShaderStageCreateInfo pixelShaderInfo = pixelShader.GetStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT, "_Baked/Jupiter_Common/Shaders/triangle.ps.spv", logicalDevice);

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderInfo, pixelShaderInfo };

		// Vertex input
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;

		// Input assembly
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		// Viewport
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapChain.GetExtent().width);
		viewport.height = static_cast<float>(swapChain.GetExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		// Scissor
		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = swapChain.GetExtent();

		// Dynamic states
		DynamicArray<VkDynamicState> dynamicStates =
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR,
			VK_DYNAMIC_STATE_LINE_WIDTH
		};
		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32>(dynamicStates.Count());
		dynamicState.pDynamicStates = dynamicStates.ConstBuffer();

		// Viewport state
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;
		viewportState.pViewports = nullptr;
		viewportState.pScissors = nullptr;

		// Rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		// Multisampling
		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		// Depth and stencil testing
		// Later

		// Color blending
		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;

		// Create pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = pipelineLayout.Get();
		pipelineInfo.renderPass = renderPass.Get();
		pipelineInfo.subpass = 0;

		if (const VkResult result = vkCreateGraphicsPipelines(logicalDevice.Get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create graphics pipeline! VkResult: %i", static_cast<uint32>(result));
			return false;
		}

		vertexShader.Shutdown(logicalDevice);
		pixelShader.Shutdown(logicalDevice);

		return true;
	}

	void Pipeline::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyPipeline(logicalDevice.Get(), m_graphicsPipeline, nullptr);
	}
}