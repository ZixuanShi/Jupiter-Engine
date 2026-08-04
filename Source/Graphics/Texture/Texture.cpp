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

    bool Texture::IsEmpty() const noexcept
    {
        return m_pixels.empty();
    }

    usize Texture::RowPitch() const noexcept
    {
        return static_cast<usize>(m_width) * sizeof(Color);
    }

    uint32 Texture::Width() const noexcept
    {
        return m_width;
    }

    uint32 Texture::Height() const noexcept
    {
        return m_height;
    }

    Color* Texture::Data() noexcept
    {
        return m_pixels.data();
    }

    const Color* Texture::Data() const noexcept
    {
        return m_pixels.data();
    }
}
