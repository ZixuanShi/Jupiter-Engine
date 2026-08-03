// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Vertex;

import jpt.Vector3;

export namespace jpt
{
    /** Must stay an aggregate and standard-layout: MetalRenderer::CreatePipeline builds the
        vertex descriptor from offsetof/sizeof of these members. */
    struct Vertex
    {
        Vec3 position;
        Vec3 normal;
    };
}
