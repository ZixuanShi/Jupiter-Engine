// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

// offsetof is a macro, and macros do not cross a module boundary -- import std cannot supply it.
#include <cstddef>

export module jpt.Vertex;

import jpt.LinearColor;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;

export namespace jpt
{
    /** Must stay an aggregate and standard-layout: the OBJ loader builds one by parenthesised
        aggregate initialisation, and the offsets below require standard layout. Member order is
        the vertex attribute order. */
    struct Vertex
    {
        Vec3 position;
        Vec3 normal;
        Vec2 uv;
        LinearColor color;

        /** The vertex descriptor's layout, owned by the type that defines it. The renderer asks
            for these rather than naming members, so renaming one here cannot quietly change what
            the GPU reads. Static member functions leave the aggregate intact. */
        [[nodiscard]] static constexpr usize PositionOffset() noexcept;
        [[nodiscard]] static constexpr usize NormalOffset()   noexcept;
        [[nodiscard]] static constexpr usize UVOffset()       noexcept;
        [[nodiscard]] static constexpr usize ColorOffset()    noexcept;
    };

    // Out of line, because offsetof needs the class complete.
    constexpr usize Vertex::PositionOffset() noexcept { return offsetof(Vertex, position); }
    constexpr usize Vertex::NormalOffset()   noexcept { return offsetof(Vertex, normal); }
    constexpr usize Vertex::UVOffset()       noexcept { return offsetof(Vertex, uv); }
    constexpr usize Vertex::ColorOffset()    noexcept { return offsetof(Vertex, color); }
}
