// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 


export module jpt.Vulkan_Shader_Pixel;

import jpt.Vulkan_Shader;

export namespace jpt::Vulkan
{
    /** Pixel shader. Process each pixel
        Output color */
    class PixelShader final : public Shader
    {
    public:
        virtual VkPipelineShaderStageCreateInfo GetStageCreateInfo() override;
    };

    VkPipelineShaderStageCreateInfo PixelShader::GetStageCreateInfo()
    {
        VkPipelineShaderStageCreateInfo shaderStageInfo{};

        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStageInfo.module = m_shaderModule;
        shaderStageInfo.pName = "main";

        return shaderStageInfo;
    }
}