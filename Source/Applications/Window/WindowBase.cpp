// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/Window/WindowBase.h"

namespace jpt
{
    void WindowBase::OnResize(std::uint32_t pixelWidth, std::uint32_t pixelHeight) noexcept
    {
        m_width  = pixelWidth;
        m_height = pixelHeight;
    }

    std::uint32_t WindowBase::GetWidth() const noexcept
    {
        return m_width;
    }

    std::uint32_t WindowBase::GetHeight() const noexcept
    {
        return m_height;
    }

    float WindowBase::GetAspectRatio() const noexcept
    {
        return (m_height > 0) ? static_cast<float>(m_width) / static_cast<float>(m_height) : 1.0f;
    }
}
