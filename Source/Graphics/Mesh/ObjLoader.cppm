// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.ObjLoader;

import jpt.Mesh;
import jpt.Path;
import jpt.TextScanner;
import jpt.TypeDefs;
import std;

export namespace jpt
{
    /** Wavefront OBJ. Reads v/vt/vn -- including the `v x y z r g b` vertex-colour extension,
        whose values are taken as linear -- and triangulates every f as a fan, which is exact
        for the convex faces exporters emit. Materials and groups are skipped.

        Returns an empty Mesh on any failure -- the caller already has to handle a file that
        parsed to nothing, so a second error channel would buy nothing. */
    [[nodiscard]] Mesh LoadObj(const Path& path);
}

/** Declared but not exported, so the whole of LoadObj's machinery reads at a glance here while
    staying unreachable by an importer. Defined in ObjLoader.cpp. */
namespace jpt::local
{
    constexpr uint32 kNoIndex = ~0u;

    /** A face corner as authored: three independent indices, kNoIndex where the face omitted
        the field. Two corners share one GPU vertex only when all three agree -- a position
        quoted with two normals is two vertices, and so is one quoted with two UVs. Colour is
        absent by design: it rides on the `v` line, so it follows the position index and can
        never disagree. */
    struct VertexKey
    {
        uint32 position = kNoIndex;
        uint32 texCoord = kNoIndex;
        uint32 normal   = kNoIndex;

        [[nodiscard]] constexpr bool operator==(const VertexKey&) const noexcept = default;
    };

    /** Handed to unordered_map explicitly rather than specialising std::hash: the key is not
        exported, and specialising for such a type is a corner of the standard with nothing to
        gain over one extra template argument.
        FNV-1a over the three words rather than their bytes -- the same mixing, a third of the
        steps, and the key is never hashed across a process boundary. */
    struct VertexKeyHash
    {
        [[nodiscard]] usize operator()(const VertexKey& key) const noexcept;
    };

    /** One face corner: "12", "12/3", "12//4" or "12/3/4". An omitted field comes back 0,
        which is never a valid OBJ index -- they are 1-based, and a negative one counts back
        from the end. */
    [[nodiscard]] bool ParseCorner(TextScanner& scanner, int32& position, int32& texCoord, int32& normal) noexcept;

    /** OBJ indices are 1-based, and a negative one counts back from the end of the list as
        it stands at this line. Returns count on anything out of range, which every caller
        already has to bounds-check anyway. */
    [[nodiscard]] usize Resolve(int32 index, usize count) noexcept;

    [[nodiscard]] bool ReadFile(const std::filesystem::path& path, std::string& text);
}
