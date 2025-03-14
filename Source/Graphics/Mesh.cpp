// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"
#include "Profiling/TimingProfiler.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

module jpt.Mesh;

import jpt.HashMap;

namespace jpt
{
    bool Mesh::Load(const File::Path& meshPath)
    {
        JPT_LOG("Loading mesh: %s", ToString(meshPath).ConstBuffer());

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, ToString(meshPath).ConstBuffer()))
        {
            JPT_ERROR("Failed to load mesh: %s", err.c_str());
            return false;
        }

        {
            JPT_SCOPED_TIMING_PROFILER("Load Mesh");

            //std::unordered_map<Vertex, uint32> uniqueVertices;
            HashMap<Vertex, uint32> uniqueVertices;

            for (const tinyobj::shape_t& shape : shapes)
            {
                for (const tinyobj::index_t& index : shape.mesh.indices)
                {
                    Vertex vertex = {};

                    // Position
                    vertex.position.x = attrib.vertices[3 * index.vertex_index + 0];
                    vertex.position.y = attrib.vertices[3 * index.vertex_index + 1];
                    vertex.position.z = attrib.vertices[3 * index.vertex_index + 2];

                    // Texture Coordinates
                    vertex.uv.x = attrib.texcoords[2 * index.texcoord_index + 0];
                    vertex.uv.y = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1];

                    // Color
                    vertex.color = { 1.0f, 1.0f, 1.0f };
                    
                    // Normal
                    vertex.normal.x = attrib.normals[3 * index.normal_index + 0];
                    vertex.normal.y = attrib.normals[3 * index.normal_index + 1];
                    vertex.normal.z = attrib.normals[3 * index.normal_index + 2];

                    if (!uniqueVertices.Has(vertex))
                    {
                        uniqueVertices[vertex] = static_cast<uint32>(m_vertices.Count());
                        m_vertices.EmplaceBack(vertex);
                    }

                    m_indices.EmplaceBack(uniqueVertices[vertex]);
                }
            }

            JPT_LOG("Vertices: %i", m_vertices.Count());
            JPT_LOG("Indices: %i", m_indices.Count());
        }

        return true;
    }
}