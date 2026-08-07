// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.Mesh;

import jpt.TypeDefs;
import jpt.Vector3;
import jpt.Vertex;
import std;

namespace jpt
{
    void GenerateNormals(Mesh& mesh) noexcept
    {
        for (usize i = 0; i + 2 < mesh.indices.size(); i += 3)
        {
            Vertex& a = mesh.vertices[mesh.indices[i]];
            Vertex& b = mesh.vertices[mesh.indices[i + 1]];
            Vertex& c = mesh.vertices[mesh.indices[i + 2]];

            const Vec3 faceNormal = (b.position - a.position).Cross(c.position - a.position);
            a.normal += faceNormal;
            b.normal += faceNormal;
            c.normal += faceNormal;
        }

        for (Vertex& vertex : mesh.vertices)
        {
            vertex.normal.Normalize();
        }
    }
}
