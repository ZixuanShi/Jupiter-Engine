// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/Window/WindowBase.h"

import jpt.Assert;

namespace jpt
{
    void WindowBase::SetCursorCaptured([[maybe_unused]] bool captured)
    {
        Debug::Assert(false, "This platform has no pointer to capture; a backend that has one hides this with its own.");
    }

    void WindowBase::OnResize(std::uint32_t pixelWidth, std::uint32_t pixelHeight) noexcept
    {
        m_width  = pixelWidth;
        m_height = pixelHeight;
    }

    float WindowBase::GetAspectRatio() const noexcept
    {
        return (m_height > 0) ? static_cast<float>(m_width) / static_cast<float>(m_height) : 1.0f;
    }
}
