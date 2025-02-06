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
		JPT_LOG("Loading mesh: %s", meshPath.ToCString().ConstBuffer());

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, meshPath.ToCString().ConstBuffer()))
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

					vertex.position.x = attrib.vertices[3 * index.vertex_index + 0];
					vertex.position.y = attrib.vertices[3 * index.vertex_index + 1];
					vertex.position.z = attrib.vertices[3 * index.vertex_index + 2];

					vertex.texCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
					vertex.texCoord.y = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1];

					vertex.color = { 1.0f, 1.0f, 1.0f };

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