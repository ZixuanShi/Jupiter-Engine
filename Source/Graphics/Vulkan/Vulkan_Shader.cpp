// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#include <vulkan/vulkan.h>

module jpt.Vulkan.Shader;

import jpt.Vulkan.LogicalDevice;

import jpt.DynamicArray;
import jpt.TypeDefs;

import jpt.File.IO;
import jpt.File.Path.Utils;

namespace jpt::Vulkan
{
    bool Shader::Load(const File::Path& path)
    {
        const File::Path shaderPath = File::FixDependencies(path);
        JPT_ASSERT(File::Exists(shaderPath), "Failed finding shader in %ls, did you compile shaders from Engine's Scripts/CompileShaders.bat?", shaderPath.ConstBuffer());

        DynamicArray<char> shaderCode = File::ReadBinaryFileArray(shaderPath);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shaderCode.Count();
        createInfo.pCode = reinterpret_cast<const uint32*>(shaderCode.Buffer());

        if (vkCreateShaderModule(LogicalDevice::GetVkDevice(), &createInfo, nullptr, &m_shaderModule) != VK_SUCCESS)
        {
            JPT_ERROR("Failed to create shader module from %ls", shaderPath.ConstBuffer());
            return false;
        }

        return true;
    }

    VkPipelineShaderStageCreateInfo Shader::GetStageCreateInfo()
    {
        JPT_ASSERT(false); 
        return VkPipelineShaderStageCreateInfo{};
    }

    void Shader::Unload()
    {
        vkDestroyShaderModule(LogicalDevice::GetVkDevice(), m_shaderModule, nullptr);
    }
}