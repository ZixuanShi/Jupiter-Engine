// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Mesh;

import jpt.TypeDefs;
import jpt.Vertex;
import std;

export namespace jpt
{
    /** Backend-neutral geometry: what a loader produces and a renderer uploads. */
    struct Mesh
    {
        std::vector<Vertex> vertices;
        std::vector<uint32> indices;
    };
}
