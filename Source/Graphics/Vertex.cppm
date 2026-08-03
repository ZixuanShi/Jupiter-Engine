// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vertex;

import jpt.LinearColor;
import jpt.Vector2;

export namespace jpt
{
    /** Must stay an aggregate and standard-layout: MetalRenderer::CreatePipeline builds the
        vertex descriptor from offsetof/sizeof of these members. */
    struct Vertex
    {
        Vec2        position;
        LinearColor color;
    };
}
