// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Vulkan.Shader.Compute;

import jpt.Vulkan.Shader;

export namespace jpt::Vulkan
{
	/** Performs computations on GPU. Frees CPU and have advantages of parallel computations */
	class ComputeShader final : public Shader
	{
	public:
		virtual VkPipelineShaderStageCreateInfo GetStageCreateInfo() override;
	};

	VkPipelineShaderStageCreateInfo ComputeShader::GetStageCreateInfo()
	{
		VkPipelineShaderStageCreateInfo shaderStageInfo{};

		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		shaderStageInfo.module = m_shaderModule;
		shaderStageInfo.pName = "main";

		return shaderStageInfo;
	}
}