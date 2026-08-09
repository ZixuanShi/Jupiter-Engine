// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Graphics/Null/RendererNull.h"

import jpt.Mesh;
import jpt.RendererBase;
import jpt.Texture;
import jpt.TypeDefs;

namespace jpt
{
    bool RendererNull::PreInit()
    {
        // By name, because nothing below ApplicationBase is virtual: the base's clear colour and
        // stats would otherwise never be set.
        return RendererBase::PreInit();
    }

    bool RendererNull::Init([[maybe_unused]] SurfaceHandle surface)
    {
        return true;
    }

    void RendererNull::Terminate()
    {
    }

    bool RendererNull::BeginFrame()
    {
        return false;
    }

    void RendererNull::EndFrame()
    {
    }

    void RendererNull::OnResize([[maybe_unused]] uint32 pixelWidth, [[maybe_unused]] uint32 pixelHeight)
    {
    }

    void RendererNull::RequestCapture()
    {
    }

    void RendererNull::DeleteCaptures()
    {
    }

    bool RendererNull::SetMesh([[maybe_unused]] const Mesh& mesh)
    {
        return true;
    }

    bool RendererNull::SetTexture([[maybe_unused]] const Texture& texture)
    {
        return true;
    }
}
