// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "ApplicationBlank.h"
#include "Applications/GetApp.h"
#include "Graphics/Renderer.h"

import jpt.Logger;
import jpt.ObjLoader;
import jpt.TextureLoader;
namespace jpt
{
    // Content is uploaded here rather than in a surface hook: the base's Init() has already
    // brought the renderer up, and no frame runs until SDL_AppInit returns.
    bool ApplicationBlank::Init()
    {
        if (!ApplicationBase::Init())
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
