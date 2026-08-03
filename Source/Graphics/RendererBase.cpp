// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.RendererBase;

import jpt.LinearColor;
import jpt.Matrix44;

namespace jpt
{
    void RendererBase::SetClearColor(const LinearColor& color) noexcept
    {
        m_clearColor = color;
    }

    void RendererBase::SetModel(const Mat44& model) noexcept
    {
        m_model = model;
    }

    void RendererBase::SetViewProjection(const Mat44& viewProjection) noexcept
    {
        m_viewProjection = viewProjection;
    }
}
