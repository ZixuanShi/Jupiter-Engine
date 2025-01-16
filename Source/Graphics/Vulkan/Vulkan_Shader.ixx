// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Shader;

import jpt.File.Path;

export namespace jpt::Vulkan
{
	/** Base class for shaders */
	class Shader
	{
	protected:
		VkShaderModule m_shaderModule = VK_NULL_HANDLE;

	public:
		virtual ~Shader() = default;

	public:
		VkPipelineShaderStageCreateInfo Load(const File::Path& path);
		void Unload();

	protected:
		virtual VkPipelineShaderStageCreateInfo GetStageCreateInfo();
	};
}