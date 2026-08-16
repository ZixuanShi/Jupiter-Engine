// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "ApplicationBlank.h"
#include "Applications/GetApp.h"

import jpt.Logger;
import jpt.ObjLoader;
import jpt.TextureLoader;

namespace jpt
{
    bool ApplicationBlank::Init()
    {
        if (!ApplicationBase::Init())
        {
            return false;
        }

        return true;
    }

    bool ApplicationBlank::OnSurfaceReady(Renderer::SurfaceHandle surface)
    {
        if (!ApplicationBase::OnSurfaceReady(surface))
        {
            return false;
        }

        if (!GetRenderer().SetMesh(LoadObj("Assets/Meshes/Mug.obj")))
        {
            Debug::Error("Failed to upload the mesh.");
            return false;
        }

        if (!GetRenderer().SetTexture(LoadTexture("Assets/Textures/Mug_BC.jpg")))
        {
            Debug::Error("Failed to upload the base colour map.");
            return false;
        }

        return true;
    }
}

JPT_SYNC_APP(ApplicationBlank)
