// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>

export module jpt.Vulkan.UniformBuffer;

import jpt.Vulkan.Buffer;

import jpt.Matrix44;

export namespace jpt::Vulkan
{
	struct alignas(16) UniformBufferObject
	{
	public:
		Matrix44 model;
		Matrix44 view;
		Matrix44 proj;

		//glm::mat4 model;
		//glm::mat4 view;
		//glm::mat4 proj;
	};

	/** Holds constant across all shaders. Think of it like a global configuration that all our shaders can read from */
	class UniformBuffer
	{
	private:
		Buffer m_buffer;
		void* m_mappedMemory = nullptr;

	public:
		bool Init();
		void Shutdown();

	public:
		VkBuffer GetHandle() const { return m_buffer.GetHandle(); }
		void* GetMappedMemory() const { return m_mappedMemory; }
	};
}