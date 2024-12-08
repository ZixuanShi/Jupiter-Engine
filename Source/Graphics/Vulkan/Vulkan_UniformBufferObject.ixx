// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

#include <glm/glm.hpp>

export module jpt.Vulkan.UniformBufferObject;

import jpt.Matrix44;

export namespace jpt::Vulkan
{
	/** Uniform buffer object used to pass data to shaders */
	struct UniformBufferObject
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};
}