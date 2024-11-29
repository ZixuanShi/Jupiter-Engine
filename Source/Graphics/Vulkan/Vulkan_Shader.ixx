// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"
#include "Debugging/Assert.h"

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Shader;

import jpt.Vulkan.LogicalDevice;

import jpt.DynamicArray;

import jpt.File.IO;
import jpt.File.Path;
import jpt.File.Path.Utils;

export namespace jpt::Vulkan
{
	/** Helper class to create Vulkan shader modules */
	class Shader
	{
	private:
		VkShaderModule m_shaderModule;

	public:
		VkPipelineShaderStageCreateInfo GetStageInfo(VkShaderStageFlagBits shaderStage, const File::Path& path, LogicalDevice logicalDevice);
		void Shutdown(LogicalDevice logicalDevice);

	private:
		VkShaderModule Create(LogicalDevice logicalDevice, const DynamicArray<char>& code);
	};

	VkPipelineShaderStageCreateInfo Shader::GetStageInfo(VkShaderStageFlagBits shaderStage, const File::Path& path, LogicalDevice logicalDevice)
	{
		const File::Path shaderPath = File::FixDependencies(path);
		JPT_ASSERT(File::Exists(shaderPath), "Failed finding shader in %ls, did you compile shaders from Engine's Scripts/CompileShaders.bat?", shaderPath.ConstBuffer());

		DynamicArray<char> shaderCode = File::ReadBinaryFileArray(shaderPath);
		m_shaderModule = Create(logicalDevice, shaderCode);

		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = shaderStage;
		shaderStageInfo.module = m_shaderModule;
		shaderStageInfo.pName = "main";

		return shaderStageInfo;
	}

	void Shader::Shutdown(LogicalDevice logicalDevice)
	{
		vkDestroyShaderModule(logicalDevice.Get(), m_shaderModule, nullptr);
	}

	VkShaderModule Shader::Create(LogicalDevice logicalDevice, const DynamicArray<char>& code)
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.Count();
		createInfo.pCode = reinterpret_cast<const uint32*>(code.ConstBuffer());

		VkShaderModule shaderModule;
		if (const VkResult result = vkCreateShaderModule(logicalDevice.Get(), &createInfo, nullptr, &shaderModule); result != VK_SUCCESS)
		{
			JPT_ERROR("Failed to create shader module! VkResult: %i", static_cast<uint32>(result));
		}

		return shaderModule;
	}
}