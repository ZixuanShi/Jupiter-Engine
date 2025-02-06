// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.GraphicsPipeline;

import jpt.Vertex;

import jpt.StaticArray;

export namespace jpt::Vulkan
{
	/** A sequence of operations that take the vertices and textures of meshes all the way to the pixels in the render targets */
	class GraphicsPipeline
	{
	private:
		VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;

	public:
		bool Init();
		void Shutdown();

	public:
		VkPipeline GetHandle() const { return m_graphicsPipeline; }

	private:
		VkPipelineVertexInputStateCreateInfo GetVertexInput() const;
		VkPipelineInputAssemblyStateCreateInfo GetInputAssembly() const;
		VkPipelineDynamicStateCreateInfo GetDynamicState() const;
		VkPipelineViewportStateCreateInfo GetViewportState() const;
		VkPipelineRasterizationStateCreateInfo GetRasterization() const;
		VkPipelineMultisampleStateCreateInfo GetMultisampling() const;
		VkPipelineColorBlendStateCreateInfo GetColorBlending() const;
		VkPipelineDepthStencilStateCreateInfo GetDepthStencil() const;

		VkVertexInputBindingDescription GetBindingDescription() const;
		StaticArray<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions() const;
	};
}