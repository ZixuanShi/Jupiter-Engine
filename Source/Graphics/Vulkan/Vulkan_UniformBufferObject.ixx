// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <glm/mat4x4.hpp>

export module jpt.Vulkan.UniformBufferObject;

export namespace jpt::Vulkan
{
	struct alignas(16) UniformBufferObject
	{
	public:
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};
}