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
        LinearColor color;
        Vec3f position;
        Vec3f normal;
        Vec2f uv;

    public:
        constexpr Vertex() = default;
        constexpr Vertex(const Vec3f& _position);
        constexpr Vertex(const Vec3f& _position, const LinearColor& _color);
        constexpr Vertex(const Vec3f& _position, const LinearColor& _color, const Vec2f& _uv, const Vec3f& _normal);

        constexpr uint64 Hash() const;
    };

    constexpr bool operator==(const Vertex& lhs, const Vertex& rhs)
    {
        return lhs.color    == rhs.color    &&
               lhs.position == rhs.position &&
               lhs.normal   == rhs.normal   &&
               lhs.uv       == rhs.uv;
    }

    constexpr Vertex::Vertex(const Vec3f& _position)
        : position(_position)
    {
    }

    constexpr Vertex::Vertex(const Vec3f& _position, const LinearColor& _color)
        : position(_position)
        , color(_color)
    {
    }

    constexpr Vertex::Vertex(const Vec3f& _position, const LinearColor& _color, const Vec2f& _uv, const Vec3f& _normal)
        : position(_position)
        , color(_color)
        , uv(_uv)
        , normal(_normal)
    {
    }

    constexpr uint64 Vertex::Hash() const
    {
        // Use a prime number as initial seed
        uint64 hash = 14695981039346656037ULL; // FNV-1a offset basis

        // Combine each component with multiplication and XOR
        hash = (hash * 1099511628211ULL) ^ jpt::Hash(position);  // FNV prime
        hash = (hash * 1099511628211ULL) ^ jpt::Hash(color);
        hash = (hash * 1099511628211ULL) ^ jpt::Hash(uv);
        hash = (hash * 1099511628211ULL) ^ jpt::Hash(normal);

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