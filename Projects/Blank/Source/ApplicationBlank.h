// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include "Graphics/Renderer.h"

import jpt.ApplicationBase;

namespace jpt
{
    /** A plain header, not a module: JPT_SYNC_APP defines jpt::GetApp(), which a plain
        header first declares, so the .cpp expanding it cannot be a module unit. Same shape as
        RendererMetal4.h, whose class derives from the jpt.RendererBase module. */
    class ApplicationBlank final : public ApplicationBase
    {
    public:
        bool Init() override;

        bool OnSurfaceReady(Renderer::SurfaceHandle surface) override;
    };
}
