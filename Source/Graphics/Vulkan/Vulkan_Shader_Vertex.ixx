// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

export module jpt.Vulkan_Shader_Vertex;

import jpt.Vulkan_Shader;

export namespace jpt::Vulkan
{
    /** Process each vertex
        Takes in world position, color, normal, texture coordinates.
        Output final position in clip coordinates */
    class VertexShader final : public Shader
    {
    public:
        virtual VkPipelineShaderStageCreateInfo GetStageCreateInfo() override;
    };

    VkPipelineShaderStageCreateInfo VertexShader::GetStageCreateInfo()
    {
        VkPipelineShaderStageCreateInfo shaderStageInfo{};

        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStageInfo.module = m_shaderModule;
        shaderStageInfo.pName = "main";

        return shaderStageInfo;
    }
}