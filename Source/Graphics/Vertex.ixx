// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <type_traits>

export module jpt.Vertex;

import jpt.Vector2;
import jpt.Vector3;
import jpt.LinearColor;
import jpt.Hash;

export namespace jpt
{
	struct Vertex
	{
	public:
		Vec3f position;
		LinearColor color;
		Vec2f texCoord;

	public:
		constexpr Vertex() = default;
		constexpr Vertex(const Vec3f& position, const LinearColor& color);
		constexpr Vertex(const Vec3f& position, const LinearColor& color, const Vec2f& texCoord);

		constexpr uint64 Hash() const;
	};

	constexpr bool operator==(const Vertex& lhs, const Vertex& rhs)
	{
		return lhs.position == rhs.position &&
			lhs.color == rhs.color &&
			lhs.texCoord == rhs.texCoord;
	}

	constexpr Vertex::Vertex(const Vec3f& position, const LinearColor& color)
		: position(position)
		, color(color)
	{
	}

	constexpr Vertex::Vertex(const Vec3f& position, const LinearColor& color, const Vec2f& texCoord)
		: position(position)
		, color(color)
		, texCoord(texCoord)
	{
	}

	constexpr uint64 Vertex::Hash() const
	{
		// Use a prime number as initial seed
		uint64 hash = 14695981039346656037ULL; // FNV-1a offset basis

		// Combine each component with multiplication and XOR
		hash = (hash * 1099511628211ULL) ^ jpt::Hash(position);  // FNV prime
		hash = (hash * 1099511628211ULL) ^ jpt::Hash(color);
		hash = (hash * 1099511628211ULL) ^ jpt::Hash(texCoord);

		return hash;
	}
}

namespace std
{
	template<>
	struct hash<jpt::Vertex>
	{
		size_t operator()(const jpt::Vertex& vertex) const
		{
			return vertex.Hash();
		}
	};
}