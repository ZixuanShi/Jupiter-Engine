// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

export module jpt.Vulkan_Shader;

import jpt.Asset;
import jpt.FilePath;

export namespace jpt::Vulkan
{
    /** Base class for shaders */
    class Shader : public Asset
    {
    protected:
        VkShaderModule m_shaderModule = VK_NULL_HANDLE;

    public:
        virtual bool Load(const File::Path& path) override;
        virtual void Unload() override;

    protected:
        virtual VkPipelineShaderStageCreateInfo GetStageCreateInfo();
    };
}