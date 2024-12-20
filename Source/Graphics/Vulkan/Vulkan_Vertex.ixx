// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <vulkan/vulkan.h>

export module jpt.Vulkan.Vertex;

import jpt.StaticArray;
import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;

export namespace jpt::Vulkan
{
	struct Vertex
	{
	public:
		Vec2f position;
		Vec3f color;

	public:
		constexpr Vertex() = default;
		constexpr Vertex(const Vec2f& position, const Vec3f& color);

	public:
		static VkVertexInputBindingDescription GetBindingDescription();
		static StaticArray<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
	};

	constexpr Vertex::Vertex(const Vec2f& position, const Vec3f& color)
		: position(position), color(color)
	{
	}

	VkVertexInputBindingDescription Vertex::GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	StaticArray<VkVertexInputAttributeDescription, 2> Vertex::GetAttributeDescriptions()
	{
		StaticArray<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

		// Position
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		// Color
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}

	DynamicArray<Vertex> g_vertices;
	DynamicArray<uint16> g_indices;
	
	//DynamicArray<Vertex> g_vertices =
	//{
	//	{ Vec2f( 0.5f,  0.5f), Vec3f(0.0f, 0.2f, 0.1f) },
	//	{ Vec2f(-0.5f,  0.5f), Vec3f(0.0f, 0.2f, 0.1f) },
	//	{ Vec2f( 0.0f, -0.5f), Vec3f(0.0f, 0.2f, 0.1f) },

	//	{ Vec2f(  0.25f, 0.0f), Vec3f(0.1f, 0.0f, 0.1f) },
	//	{ Vec2f(  0.0f,  0.5f), Vec3f(0.1f, 0.0f, 0.1f) },
	//	{ Vec2f( -0.25f, 0.0f), Vec3f(0.1f, 0.0f, 0.1f) },
	//};

	//DynamicArray<uint16> g_indices =
	//{
	//	0, 1, 2, 
	//	3, 4, 5
	//};
}