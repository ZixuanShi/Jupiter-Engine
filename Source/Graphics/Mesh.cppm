// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Mesh;

import jpt.Asset;

import jpt.Vertex;

import jpt.DynamicArray;
import jpt.TypeDefs;

import jpt.FilePath;

export namespace jpt
{
    /** 3D Mesh in world. Collection of vertices, edges, faces */
    class Mesh final : public Asset
    {
    private:
        DynamicArray<Vertex> m_vertices;
        DynamicArray<uint32> m_indices;

    public:
        bool Load(const File::Path& fullPath) override;

    public:
        const DynamicArray<Vertex>& GetVertices() const { return m_vertices; }
        const DynamicArray<uint32>& GetIndices() const { return m_indices; }
    };
}