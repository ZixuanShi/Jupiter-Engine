// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.RendererBase;

import jpt.LinearColor;

export namespace jpt
{
    /** State every backend needs and none of them implements differently. */
    class RendererBase
    {
    protected:
        LinearColor m_clearColor = LinearColor::Black();

    public:
        void SetClearColor(const LinearColor& color) noexcept { m_clearColor = color; }
    };
}
