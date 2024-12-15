// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.GraphicsPipeline;

import jpt.Vulkan.Data;
import jpt.Vulkan.LogicalDevice;
import jpt.Vulkan.Shader.Vertex;
import jpt.Vulkan.Shader.Pixel;
import jpt.Vulkan.PipelineLayout;
import jpt.Vulkan.RenderPass;

import jpt.DynamicArray;
import jpt.StaticArray;
import jpt.TypeDefs;

export namespace jpt::Vulkan
{
	/** A sequence of operations that take the vertices and textures of meshes all the way to the pixels in the render targets */
	class GraphicsPipeline
	{
	private:
		VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;

	public:
		bool Init(const LogicalDevice& logicalDevice, const PipelineLayout& pipelineLayout, const RenderPass& renderPass);
		void Shutdown(const LogicalDevice& logicalDevice);

	public:
		VkPipeline GetHandle() const { return m_graphicsPipeline; }

	private:
		VkPipelineVertexInputStateCreateInfo GetVertexInput(const VkVertexInputBindingDescription& inputBindingDesc, const StaticArray<VkVertexInputAttributeDescription, 2>& attributeDescs) const;
		VkPipelineInputAssemblyStateCreateInfo GetInputAssembly() const;
		VkPipelineDynamicStateCreateInfo GetDynamicState(const StaticArray<VkDynamicState, 2>& dynamicStates) const;
		VkPipelineViewportStateCreateInfo GetViewportState() const;
		VkPipelineRasterizationStateCreateInfo GetRasterization() const;
		VkPipelineMultisampleStateCreateInfo GetMultisampling() const;
		VkPipelineColorBlendStateCreateInfo GetColorBlending(const VkPipelineColorBlendAttachmentState& attachment) const;
	};

	bool GraphicsPipeline::Init(const LogicalDevice& logicalDevice, const PipelineLayout& pipelineLayout, const RenderPass& renderPass)
	{
		// Shaders. Programmable stages of the pipeline.
		VertexShader vertexShader;
		PixelShader pixelShader;

		VkPipelineShaderStageCreateInfo vertexShaderStageInfo = vertexShader.Load("_Baked/Jupiter_Common/Shaders/triangle.vs.spv", logicalDevice);
		VkPipelineShaderStageCreateInfo pixelShaderStageInfo = pixelShader.Load("_Baked/Jupiter_Common/Shaders/triangle.ps.spv", logicalDevice);

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, pixelShaderStageInfo };

		// Fixed-function stages
		VkVertexInputBindingDescription bindingDescription = Vertex::GetBindingDescription();
		StaticArray<VkVertexInputAttributeDescription, 2> attributeDescriptions = Vertex::GetAttributeDescriptions();
		auto vertexInputInfo = GetVertexInput(bindingDescription, attributeDescriptions);

		auto inputAssembly = GetInputAssembly();
		auto viewportState = GetViewportState();
		auto rasterizer = GetRasterization();
		auto multisampling = GetMultisampling();

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		auto colorBlending = GetColorBlending(colorBlendAttachment);

		StaticArray<VkDynamicState, 2> dynamicStates =
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};
		auto dynamicState = GetDynamicState(dynamicStates);

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = pipelineLayout.GetHandle();
		pipelineInfo.renderPass = renderPass.GetHandle();
		pipelineInfo.subpass = 0;

		if (const VkResult result = vkCreateGraphicsPipelines(logicalDevice.GetHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create graphics pipeline: %d", result);
			return false;
		}

		vertexShader.Unload(logicalDevice);
		pixelShader.Unload(logicalDevice);

		return true;
	}

	void GraphicsPipeline::Shutdown(const LogicalDevice& logicalDevice)
	{
		vkDestroyPipeline(logicalDevice.GetHandle(), m_graphicsPipeline, nullptr);
	}

	VkPipelineVertexInputStateCreateInfo GraphicsPipeline::GetVertexInput(const VkVertexInputBindingDescription& inputBindingDesc, const StaticArray<VkVertexInputAttributeDescription, 2>& attributeDescs) const
	{
		// how to interpret vertex data from your vertex buffers
		// Binding Description: spacing between data and whether the data is per-vertex or per-instance
		// Attribute Descriptions: type of the attributes passed to the vertex shader, which binding to load them from and at which offset

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &inputBindingDesc;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32>(attributeDescs.Count());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescs.ConstBuffer();

		return vertexInputInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo GraphicsPipeline::GetInputAssembly() const
	{
		// Defines how to interpret vertex data and assemble it into primitives

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		return inputAssembly;
	}

	VkPipelineDynamicStateCreateInfo GraphicsPipeline::GetDynamicState(const StaticArray<VkDynamicState, 2>& dynamicStates) const
	{
		// Allows certain states of the pipeline to be changed without recreating the pipeline

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = 2;
		dynamicState.pDynamicStates = dynamicStates.ConstBuffer();

		return dynamicState;
	}

	VkPipelineViewportStateCreateInfo GraphicsPipeline::GetViewportState() const
	{
		// Describes the viewport and scissor rectangle that the rasterizer will use when rendering
		// Viewport: Describes the region of the framebuffer that the output will be rendered to
		// Scissor: Defines which regions pixels will actually be stored. Any pixels outside the scissor rectangle will be discarded by the rasterizer

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = nullptr;
		viewportState.scissorCount = 1;
		viewportState.pScissors = nullptr;

		return viewportState;
	}

	VkPipelineRasterizationStateCreateInfo GraphicsPipeline::GetRasterization() const
	{
		// Takes the geometry that is shaped by the vertices from the vertex shader and turns it into fragments to be colored by the fragment shader
		// Also performs depth testing, face culling and the scissor test

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		return rasterizer;
	}

	VkPipelineMultisampleStateCreateInfo GraphicsPipeline::GetMultisampling() const
	{
		// Multisampling is one of the ways to perform anti-aliasing
		// It works by combining the fragment shader results of multiple polygons that rasterize to the same pixel

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		return multisampling;
	}

	VkPipelineColorBlendStateCreateInfo GraphicsPipeline::GetColorBlending(const VkPipelineColorBlendAttachmentState& attachment) const
	{
		/** After a fragment shader has returned a color, it needs to be combined with the color that is already in the framebuffer. 
			This transformation is known as color blending and there are two ways to do it:
			- Mix the old and new value to produce a final color
			- Combine the old and new value using a bitwise operation
		*/

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &attachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		return colorBlending;
	}
}