// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

#include <glm/mat4x4.hpp>

export module jpt.Vulkan.UniformBufferObject;

export namespace jpt::Vulkan
{
	struct UniformBufferObject
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};
}