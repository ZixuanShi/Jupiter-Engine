// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.ObjLoader;

import jpt.Logger;
import jpt.Mesh;
import jpt.Path;
import jpt.StringUtils;
import jpt.TypeDefs;
import jpt.Vector3;
import jpt.Vertex;
import std;

namespace jpt
{
    namespace
    {
        constexpr uint32 kNoNormal = ~0u;

        void SkipBlanks(std::string_view& text) noexcept
        {
            const usize start = text.find_first_not_of(" \t");
            text.remove_prefix(start == std::string_view::npos ? text.size() : start);
        }

        /** Consumes a leading '+' or '-' if there is one. */
        [[nodiscard]] bool ParseSign(std::string_view& text) noexcept
        {
            const bool negative = text.starts_with('-');
            if (negative || text.starts_with('+'))
            {
                text.remove_prefix(1);
            }
            return negative;
        }

        /** Consumes a run of digits into value, most significant first. Returns how many. */
        usize ParseDigits(std::string_view& text, float64& value) noexcept
        {
            usize count = 0;
            for (; !text.empty() && IsDigit(text.front()); ++count)
            {
                value = value * 10.0 + static_cast<float64>(text.front() - '0');
                text.remove_prefix(1);
            }
            return count;
        }

        /** Every parser here stops at the first character that is not part of the number, which
            is what lets one call read a field out of "12/3/4" and leave the separator behind. */
        [[nodiscard]] bool Parse(std::string_view& text, int32& value) noexcept
        {
            SkipBlanks(text);

            const std::from_chars_result result = std::from_chars(text.data(), text.data() + text.size(), value);
            if (result.ec != std::errc{})
            {
                return false;
            }

            text.remove_prefix(static_cast<usize>(result.ptr - text.data()));
            return true;
        }

        /** Hand-rolled because libc++ puts the floating-point from_chars in the dylib behind a
            macOS 26 availability guard, which our deployment target does not clear. strtof is
            no substitute: it reads the decimal point from a locale, and taking no end pointer
            it would run off the end of one line and into the next. */
        [[nodiscard]] bool Parse(std::string_view& text, float32& value) noexcept
        {
            SkipBlanks(text);

            std::string_view cursor = text;
            const bool negative = ParseSign(cursor);

            // Accumulated as one integer and scaled once, rather than adding each fraction
            // digit at its own magnitude -- half the rounding steps for the same digits.
            float64 mantissa = 0.0;
            usize digits = ParseDigits(cursor, mantissa);

            if (cursor.starts_with('.'))
            {
                cursor.remove_prefix(1);

                const usize beforeFraction = cursor.size();
                digits += ParseDigits(cursor, mantissa);
                mantissa /= std::pow(10.0, static_cast<float64>(beforeFraction - cursor.size()));
            }

            if (digits == 0)
            {
                return false;
            }

            if (cursor.starts_with('e') || cursor.starts_with('E'))
            {
                std::string_view exponentCursor = cursor.substr(1);
                const bool exponentNegative = ParseSign(exponentCursor);

                float64 exponent = 0.0;
                if (ParseDigits(exponentCursor, exponent) > 0)
                {
                    mantissa *= std::pow(10.0, exponentNegative ? -exponent : exponent);
                    cursor = exponentCursor;
                }
            }

            value = static_cast<float32>(negative ? -mantissa : mantissa);
            text = cursor;
            return true;
        }

        /** One face corner: "12", "12/3", "12//4" or "12/3/4". */
        [[nodiscard]] bool ParseCorner(std::string_view& text, int32& position, int32& normal) noexcept
        {
            normal = 0;

            if (!Parse(text, position))
            {
                return false;
            }

            if (text.starts_with('/'))
            {
                text.remove_prefix(1);

                int32 texCoord = 0;
                (void)Parse(text, texCoord);

                if (text.starts_with('/'))
                {
                    text.remove_prefix(1);
                    return Parse(text, normal);
                }
            }

            return true;
        }

        /** OBJ indices are 1-based, and a negative one counts back from the end of the list as
            it stands at this line. Returns count on anything out of range, which every caller
            already has to bounds-check anyway. */
        [[nodiscard]] usize Resolve(int32 index, usize count) noexcept
        {
            const int64 resolved = (index > 0) ? index - 1 : static_cast<int64>(count) + index;
            return (index != 0 && resolved >= 0 && static_cast<usize>(resolved) < count) ? static_cast<usize>(resolved) : count;
        }

        /** Area-weighted: the raw cross product's length is twice the triangle's area, so
            summing it unnormalized already gives larger faces a proportionally larger say. */
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

        [[nodiscard]] bool ReadFile(const std::filesystem::path& path, std::string& text)
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

    Mesh LoadObj(const Path& path)
    {
        std::string text;
        if (!ReadFile(path.GetAbsolute(), text))
        {
            Debug::Error("Cannot read {}", path.GetAbsolute().string());
            return {};
        }

        std::vector<Vec3> positions;
        std::vector<Vec3> normals;

        // (position, normal) -> index into mesh.vertices. A shared corner with two different
        // normals is two GPU vertices; with no normals at all the key collapses to position,
        // which is exactly the sharing GenerateNormals below wants.
        std::unordered_map<uint64, uint32> unique;

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
            SkipBlanks(line);

            if (line.starts_with("v "))
            {
                line.remove_prefix(1);

                Vec3 position;
                if (Parse(line, position.x) && Parse(line, position.y) && Parse(line, position.z))
                {
                    positions.push_back(position);
                }
            }
            else if (line.starts_with("vn "))
            {
                line.remove_prefix(2);

                Vec3 normal;
                if (Parse(line, normal.x) && Parse(line, normal.y) && Parse(line, normal.z))
                {
                    normals.push_back(normal);
                }
            }
            else if (line.starts_with("f "))
            {
                line.remove_prefix(1);
                corners.clear();

                for (int32 position = 0, normal = 0; ParseCorner(line, position, normal); )
                {
                    const usize positionIndex = Resolve(position, positions.size());
                    if (positionIndex == positions.size())
                    {
                        Debug::Error("{}: face references vertex {}, which does not exist.", path.GetFileName(), position);
                        return {};
                    }

                    const usize normalIndex = (normal != 0) ? Resolve(normal, normals.size()) : normals.size();
                    const uint32 packedNormal = (normalIndex < normals.size()) ? static_cast<uint32>(normalIndex) : kNoNormal;
                    const uint64 key = (static_cast<uint64>(positionIndex) << 32) | packedNormal;

                    const auto [it, inserted] = unique.try_emplace(key, static_cast<uint32>(mesh.vertices.size()));
                    if (inserted)
                    {
                        mesh.vertices.emplace_back(positions[positionIndex],
                                                   (packedNormal != kNoNormal) ? normals[packedNormal] : Vec3::Zero());
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
}
