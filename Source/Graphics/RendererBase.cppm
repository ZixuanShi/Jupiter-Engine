// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.RendererBase;

import jpt.LinearColor;

export namespace jpt
{
    class RendererBase
    {
    protected:
        LinearColor m_clearColor = LinearColor::Black();

    public:
        void SetClearColor(const LinearColor& color) noexcept;
    };
}
