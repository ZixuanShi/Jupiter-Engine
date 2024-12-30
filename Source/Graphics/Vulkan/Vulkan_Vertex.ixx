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
		Vec3f position;
		Vec3f color;
		Vec2f texCoord;

	public:
		constexpr Vertex() = default;
		constexpr Vertex(const Vec3f& position, const Vec3f& color);
		constexpr Vertex(const Vec3f& position, const Vec3f& color, const Vec2f& texCoord);

	public:
		static VkVertexInputBindingDescription GetBindingDescription();
		static StaticArray<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
	};

	constexpr Vertex::Vertex(const Vec3f& position, const Vec3f& color)
		: position(position)
		, color(color)
	{
	}

	constexpr Vertex::Vertex(const Vec3f& position, const Vec3f& color, const Vec2f& texCoord)
		: position(position)
		, color(color)
		, texCoord(texCoord)
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

	StaticArray<VkVertexInputAttributeDescription, 3> Vertex::GetAttributeDescriptions()
	{
		StaticArray<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		// Position
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		// Color
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		// TexCoord
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}

	//DynamicArray<Vertex> g_vertices;
	//DynamicArray<uint16> g_indices;
	
	DynamicArray<Vertex> g_vertices =
	{
		  // Position             // Color              // TexCoord
		{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
		{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
		{ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
		{ { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },

		{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
		{ {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
		{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }
	};

	DynamicArray<uint16> g_indices =
	{
		0, 1, 2, 
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
	};
}