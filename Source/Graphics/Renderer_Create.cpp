// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Memory/Memory.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

module jpt.Renderer_Create;

import jpt.Renderer;
import jpt.Graphics_Enums;
import jpt.Renderer_Vulkan;
import jpt.Renderer_DX12;

import jpt.AppSettings;

namespace jpt
{
    GraphicsAPI FindGraphicsAPI()
    {
#if IS_PLATFORM_WINDOWS
        const GraphicsAPI api = GetSettings<GraphicsAPI>("graphicsAPI", GraphicsAPI::Vulkan);
#endif

        JPT_ASSERT(api != GraphicsAPI::Unknown, "No Graphics API specified in CommandLine or ProjectSettings.json.");
        return api;
    }

    Renderer* Renderer_Create()
    {
        const GraphicsAPI api = FindGraphicsAPI();

        switch (api.Value())
        {
        case GraphicsAPI::Vulkan:
            return JPT_NEW(Renderer_Vulkan);

        case GraphicsAPI::DX12:
            return JPT_NEW(Renderer_DX12);

        default:
            JPT_ERROR("Un-implemented Graphics API: " + ToString(api));
            return nullptr;
        }
    }
}