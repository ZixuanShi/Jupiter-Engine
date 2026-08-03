// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.RendererBase;

import jpt.LinearColor;
import jpt.Matrix44;

export namespace jpt
{
    class RendererBase
    {
    protected:
        Mat44 m_model            = Mat44::Identity();
        Mat44 m_viewProjection   = Mat44::Identity();
        LinearColor m_clearColor = LinearColor::Black();

    public:
        void SetClearColor(const LinearColor& color) noexcept;
        void SetModel(const Mat44& model) noexcept;
        void SetViewProjection(const Mat44& viewProjection) noexcept;
    };
}
