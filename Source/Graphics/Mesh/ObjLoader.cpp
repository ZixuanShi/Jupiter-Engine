// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.ObjLoader;

import jpt.LinearColor;
import jpt.Logger;
import jpt.Mesh;
import jpt.Path;
import jpt.TextScanner;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;
import jpt.Vertex;
import std;

namespace jpt
{
    Mesh LoadObj(const Path& path)
    {
        std::string text;
        if (!local::ReadFile(path.GetAbsolute(), text))
        {
            Debug::Error("Cannot read {}", path.GetAbsolute().string());
            return {};
        }

        std::vector<Vec3> positions;
        std::vector<Vec3> normals;
        std::vector<Vec2> texCoords;
        std::vector<LinearColor> colors;   // Parallel to positions: the colour extension is on `v`.

        // Corner -> index. A file with UVs but no normals splits at its UV seams *before*
        // GenerateNormals runs, so those seams come out faceted.
        std::unordered_map<local::VertexKey, uint32, local::VertexKeyHash> unique;

        Mesh mesh;
        std::vector<uint32> corners;

        std::string_view remaining(text);
        while (!remaining.empty())
        {
            const usize lineEnd = remaining.find('\n');
            std::string_view line = remaining.substr(0, lineEnd);
            remaining.remove_prefix(lineEnd == std::string_view::npos ? remaining.size() : lineEnd + 1);

            if (line.ends_with('\r'))
            {
                line.remove_suffix(1);
            }

            TextScanner scanner(line);
            scanner.SkipBlanks();

            if (scanner.Consume("v "))
            {
                Vec3 position;
                if (scanner.Parse(position.x) && scanner.Parse(position.y) && scanner.Parse(position.z))
                {
                    positions.push_back(position);

                    // The `v x y z r g b` vertex-colour extension. All three or none, so the
                    // spec's optional fourth `w` component cannot be mistaken for a red channel.
                    float32 r = 0.0f;
                    float32 g = 0.0f;
                    float32 b = 0.0f;
                    const bool hasColor = scanner.Parse(r) && scanner.Parse(g) && scanner.Parse(b);
                    colors.push_back(hasColor ? LinearColor(r, g, b) : LinearColor::White());
                }
            }
            else if (scanner.Consume("vt "))
            {
                Vec2 texCoord;
                if (scanner.Parse(texCoord.x) && scanner.Parse(texCoord.y))
                {
                    texCoords.push_back(texCoord);
                }
            }
            else if (scanner.Consume("vn "))
            {
                Vec3 normal;
                if (scanner.Parse(normal.x) && scanner.Parse(normal.y) && scanner.Parse(normal.z))
                {
                    normals.push_back(normal);
                }
            }
            else if (scanner.Consume("f "))
            {
                corners.clear();

                for (int32 position = 0, texCoord = 0, normal = 0; local::ParseCorner(scanner, position, texCoord, normal); )
                {
                    const usize positionIndex = local::Resolve(position, positions.size());
                    if (positionIndex == positions.size())
                    {
                        Debug::Error("{}: face references vertex {}, which does not exist.", path.GetFileName(), position);
                        return {};
                    }

                    local::VertexKey key;
                    key.position = static_cast<uint32>(positionIndex);

                    // Unlike the position, an out-of-range UV or normal is treated as absent
                    // rather than fatal -- the vertex still has somewhere to be.
                    if (const usize index = local::Resolve(texCoord, texCoords.size()); index < texCoords.size())
                    {
                        key.texCoord = static_cast<uint32>(index);
                    }
                    if (const usize index = local::Resolve(normal, normals.size()); index < normals.size())
                    {
                        key.normal = static_cast<uint32>(index);
                    }

                    const auto [it, inserted] = unique.try_emplace(key, static_cast<uint32>(mesh.vertices.size()));
                    if (inserted)
                    {
                        mesh.vertices.emplace_back(positions[key.position],
                                                   (key.normal   != local::kNoIndex) ? normals[key.normal]     : Vec3::Zero(),
                                                   (key.texCoord != local::kNoIndex) ? texCoords[key.texCoord] : Vec2::Zero(),
                                                   colors[key.position]);
                    }

                    corners.push_back(it->second);
                }

                for (usize i = 2; i < corners.size(); ++i)
                {
                    mesh.indices.insert(mesh.indices.end(), { corners[0], corners[i - 1], corners[i] });
                }
            }
        }

        if (normals.empty())
        {
            GenerateNormals(mesh);
        }

        Debug::Info("Loaded {}: {} vertices, {} triangles.", path.GetFileName(), mesh.vertices.size(), mesh.indices.size() / 3);
        return mesh;
    }

    namespace local
    {
        usize VertexKeyHash::operator()(const VertexKey& key) const noexcept
        {
            uint64 hash = 14695981039346656037ull;
            for (const uint32 index : { key.position, key.texCoord, key.normal })
            {
                hash = (hash ^ index) * 1099511628211ull;
            }
            return static_cast<usize>(hash);
        }

        bool ParseCorner(TextScanner& scanner, int32& position, int32& texCoord, int32& normal) noexcept
        {
            texCoord = 0;
            normal   = 0;

            if (!scanner.Parse(position))
            {
                return false;
            }

            if (scanner.Consume('/'))
            {
                (void)scanner.Parse(texCoord);   // "12//4": the parse fails on '/' and leaves the 0.

                if (scanner.Consume('/'))
                {
                    return scanner.Parse(normal);
                }
            }

            return true;
        }

        usize Resolve(int32 index, usize count) noexcept
        {
            const int64 resolved = (index > 0) ? index - 1 : static_cast<int64>(count) + index;
            return (index != 0 && resolved >= 0 && static_cast<usize>(resolved) < count) ? static_cast<usize>(resolved) : count;
        }

        bool ReadFile(const std::filesystem::path& path, std::string& text)
        {
            std::ifstream file(path, std::ios::binary | std::ios::ate);
            if (!file)
            {
                return false;
            }

            text.resize(static_cast<usize>(file.tellg()));
            file.seekg(0);
            return static_cast<bool>(file.read(text.data(), static_cast<std::streamsize>(text.size())));
        }
    }
}
