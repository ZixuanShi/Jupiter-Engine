// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan_GraphicsPipeline;

import jpt.Application;
import jpt.Renderer_Vulkan;

import jpt.Vulkan_PhysicalDevice;
import jpt.Vulkan_LogicalDevice;
import jpt.Vulkan_PipelineLayout;
import jpt.Vulkan_RenderPass;

import jpt.Vulkan_Shader_Vertex;
import jpt.Vulkan_Shader_Pixel;
import jpt.Vulkan_Shader_Compute;

import jpt.DynamicArray;
import jpt.TypeDefs;

namespace jpt::Vulkan
{
    bool GraphicsPipeline::Init()
    {
        const Renderer_Vulkan* pVulkanRenderer = GetVkRenderer();
        const PipelineLayout& pipelineLayout = pVulkanRenderer->GetPipelineLayout();
        const RenderPass& renderPass = pVulkanRenderer->GetRenderPass();

        // Shaders. Programmable stages of the pipeline.
        VertexShader  vertexShader;
        PixelShader   pixelShader;

        JPT_ASSERT(vertexShader.Load("_Baked/Jupiter_Common/Shaders/Default_Vert.glsl.spv"));
        JPT_ASSERT(pixelShader.Load("_Baked/Jupiter_Common/Shaders/Default_Frag.glsl.spv"));

        VkPipelineShaderStageCreateInfo vertexShaderStageInfo  = vertexShader.GetStageCreateInfo();
        VkPipelineShaderStageCreateInfo pixelShaderStageInfo   = pixelShader.GetStageCreateInfo();

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, pixelShaderStageInfo };

        // Fixed-function stages
        auto inputAssembly   = GetInputAssembly();
        auto vertexInputInfo = GetVertexInput();
        auto viewportState   = GetViewportState();
        auto rasterizer      = GetRasterization();
        auto multisampling   = GetMultisampling();
        auto depthStencil    = GetDepthStencil();
        auto colorBlending   = GetColorBlending();
        auto dynamicState    = GetDynamicState();

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = pipelineLayout.GetHandle();
        pipelineInfo.renderPass = renderPass.GetHandle();
        pipelineInfo.subpass = 0;

        if (const VkResult result = vkCreateGraphicsPipelines(LogicalDevice::GetVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline); result != VK_SUCCESS)
        {
            JPT_ERROR("Failed to create graphics pipeline: %d", result);
            return false;
        }

        vertexShader.Unload();
        pixelShader.Unload();

        return true;
    }

    void GraphicsPipeline::Shutdown()
    {
        vkDestroyPipeline(LogicalDevice::GetVkDevice(), m_graphicsPipeline, nullptr);
    }

    VkPipelineVertexInputStateCreateInfo GraphicsPipeline::GetVertexInput() const
    {
        // how to interpret vertex data from your vertex buffers
        // Binding Description: spacing between data and whether the data is per-vertex or per-instance
        // Attribute Descriptions: type of the attributes passed to the vertex shader, which binding to load them from and at which offset

        static VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        static StaticArray<VkVertexInputAttributeDescription, 4> attributeDescriptions
        {
            // location, binding, format,                     offset
            { 0,         0,       VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)    }, // Color
            { 1,         0,       VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) }, // Position
            { 2,         0,       VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)   }, // Normal
            { 3,         0,       VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, uv)       }, // uv
        };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32>(attributeDescriptions.Count());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.ConstBuffer();

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
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        return rasterizer;
    }

    VkPipelineMultisampleStateCreateInfo GraphicsPipeline::GetMultisampling() const
    {
        // Multisampling is one of the ways to perform anti-aliasing
        // It works by combining the fragment shader results of multiple polygons that rasterize to the same pixel

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_TRUE;
        multisampling.rasterizationSamples = PhysicalDevice::Get().GetMsaaSamples();
        multisampling.minSampleShading = 0.2f; // Optional

        return multisampling;
    }

    VkPipelineDepthStencilStateCreateInfo GraphicsPipeline::GetDepthStencil() const
    {
        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional

        return depthStencil;
    }

    VkPipelineColorBlendStateCreateInfo GraphicsPipeline::GetColorBlending() const
    {
        /** After a fragment shader has returned a color, it needs to be combined with the color that is already in the framebuffer.
            This transformation is known as color blending and there are two ways to do it:
            - Mix the previous and current value to produce a final color
            - Combine the previous and current value using a bitwise operation
        */

        static VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        return colorBlending;
    }

    VkPipelineDynamicStateCreateInfo GraphicsPipeline::GetDynamicState() const
    {
        // Allows certain states of the pipeline to be changed without recreating the pipeline

        static StaticArray<VkDynamicState, 2> dynamicStates =
        {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = 2;
        dynamicState.pDynamicStates = dynamicStates.ConstBuffer();

        return dynamicState;
    }
}