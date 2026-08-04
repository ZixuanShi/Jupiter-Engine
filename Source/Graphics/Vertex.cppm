// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vertex;

import jpt.LinearColor;
import jpt.Vector2;
import jpt.Vector3;

export namespace jpt
{
    /** Must stay an aggregate and standard-layout: Metal4Renderer::CreatePipeline builds the
        vertex descriptor from offsetof/sizeof of these members, and the OBJ loader builds one
        by parenthesised aggregate initialisation. Member order is the attribute order. */
    struct Vertex
    {
        Vec3 position;
        Vec3 normal;
        Vec2 uv;
        LinearColor color;
    };
}
