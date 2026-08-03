// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.RendererBase;

import jpt.LinearColor;

namespace jpt
{
    void RendererBase::SetClearColor(const LinearColor& color) noexcept
    {
        m_clearColor = color;
    }
}
