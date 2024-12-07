// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Vertex;

import jpt.Vector2;
import jpt.Vector3;

import jpt.StaticArray;
import jpt.DynamicArray;

export namespace jpt::Vulkan
{
	struct Vertex2
	{
		Vec2f position;
		Vec3f color;

		static VkVertexInputBindingDescription GetBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex2);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDescription;
		}

		static StaticArray<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
		{
			StaticArray<VkVertexInputAttributeDescription, 2> attributeDescriptions;

			// Position
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex2, position);

			// Color
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex2, color);

			return attributeDescriptions;
		}
	};

	const DynamicArray<Vertex2> vertices =
	{
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};
}