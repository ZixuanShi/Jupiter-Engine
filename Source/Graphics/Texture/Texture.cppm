// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Texture;

import jpt.Color;
import jpt.TypeDefs;
import std;

export namespace jpt
{
    /** Binding order. The renderer indexes its texture array by this, and the shader's
        [[texture(n)]] indices match. */
    enum class TextureSlot : uint8
    {
        BaseColor,
        Normal,
        Roughness,
        Metallic,
        Occlusion,

        Count,
    };

    constexpr usize kTextureSlotCount = static_cast<usize>(TextureSlot::Count);

    /** Backend-neutral pixels: what a loader produces and a renderer uploads, mirroring Mesh.
        The GPU-side object lives inside the backend, as vertex and index buffers do.

        Always RGBA8. Metal has no three-channel format, so the RGB maps have to expand anyway,
        and expanding the single-channel ones too keeps one upload path -- the shader reads .r. */
    class Texture
    {
    private:
        std::vector<Color> m_pixels;
        uint32 m_width  = 0;
        uint32 m_height = 0;

    public:
        /** Sets the dimensions and the pixel count together, so they cannot disagree. */
        void Resize(uint32 width, uint32 height);

        [[nodiscard]] bool IsEmpty() const noexcept;

        /** Bytes per row, which is the unit both CGBitmapContextCreate and replaceRegion want.
            The pixels are Colors rather than bytes, so this is the one place that crosses
            between the two counts. */
        [[nodiscard]] usize RowPitch() const noexcept;

        [[nodiscard]] uint32 Width()  const noexcept;
        [[nodiscard]] uint32 Height() const noexcept;

        // Non-const for the decoder, which draws straight into this storage.
        [[nodiscard]] Color* Data() noexcept;
        [[nodiscard]] const Color* Data() const noexcept;
    };
}
