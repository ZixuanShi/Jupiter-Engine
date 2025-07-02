// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module jpt.Renderer_Create;

import jpt.Renderer;
import jpt.Graphics_Enums;
import jpt.Renderer_Vulkan;
import jpt.Renderer_DX12;

import jpt.CommandLine;
import jpt.ProjectSettings;

export namespace jpt
{
    GraphicsAPI FindGraphicsAPI()
    {
        GraphicsAPI api = GraphicsAPI::Unknown;
        const String graphicsApiKey = "graphicsAPI";

        // Check CommandLine for GraphicsAPI
        if (CommandLine::GetInstance().Has(graphicsApiKey))
        {
            api = CommandLine::GetInstance().Get<String>(graphicsApiKey);
        }
        // Check Config/ProjectSettings.json project settings
        else if (String graphicsApi; ProjectSettings::GetInstance().TryGet(graphicsApiKey, graphicsApi))
        {
            api = graphicsApi;
        }
        // Default based on platform
        else
        {
#if IS_PLATFORM_WINDOWS
            api = GraphicsAPI::Vulkan;
#endif
        }

        JPT_ASSERT(api != GraphicsAPI::Unknown, "No Graphics API specified in CommandLine or ProjectSettings.json.");
        return api;
    }

    Renderer* Renderer_Create(GraphicsAPI api)
    {
        switch (api.Value())
        {
        case GraphicsAPI::Vulkan:
            return new Renderer_Vulkan();

        case GraphicsAPI::DX12:
            return new Renderer_DX12();

        default:
            JPT_ERROR("Un-implemented Graphics API: " + ToString(api));
            return nullptr;
        }
    }
}