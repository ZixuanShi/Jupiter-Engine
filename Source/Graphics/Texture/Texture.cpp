// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.Texture;

import jpt.Color;
import jpt.TypeDefs;
import std;

namespace jpt
{
    void Texture::Resize(uint32 width, uint32 height)
    {
        m_width  = width;
        m_height = height;
        m_pixels.resize(static_cast<usize>(width) * height);
    }

    usize Texture::RowPitch() const noexcept
    {
        return static_cast<usize>(m_width) * sizeof(Color);
    }
}
