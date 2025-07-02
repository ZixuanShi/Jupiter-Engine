// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan_GraphicsPipeline;

import jpt.Vertex;
import jpt.ArrayView;

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
        // 1. Input stage - defines vertex data format
        VkPipelineVertexInputStateCreateInfo   GetVertexInput(const VkVertexInputBindingDescription& bindingDescription, ArrayView<VkVertexInputAttributeDescription> attributeDescriptions) const;

        // 2. Input Assembly - how to interpret vertices (triangles, lines, etc)
        VkPipelineInputAssemblyStateCreateInfo GetInputAssembly() const;

        // 3. Viewport & Scissors - where to render
        VkPipelineViewportStateCreateInfo      GetViewportState() const;

        // 4. Rasterization - converts primitives to fragments
        VkPipelineRasterizationStateCreateInfo GetRasterization() const;

        // 5. Multisampling - anti-aliasing settings
        VkPipelineMultisampleStateCreateInfo   GetMultisampling() const;

        // 6. Depth & Stencil testing
        VkPipelineDepthStencilStateCreateInfo  GetDepthStencil() const;

        // 7. Color blending - how to combine with render target
        VkPipelineColorBlendStateCreateInfo    GetColorBlending() const;

        // Unordered. Dynamic state - allows changing some settings without recreating the pipeline
        VkPipelineDynamicStateCreateInfo       GetDynamicState() const;
    };
}