// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module jpt.Framework_Create;

import jpt.Framework;
import jpt.Framework_Enums;
import jpt.Framework_GLFW;
import jpt.Framework_Win32;

import jpt.CommandLine;
import jpt.ProjectSettings;

export namespace jpt
{
    FrameworkAPI FindFrameworkAPI()
    {
        FrameworkAPI api = FrameworkAPI::Unknown;
        const String frameworkApiKey = "frameworkAPI";

        // Check CommandLine for FrameworkAPI
        if (CommandLine::GetInstance().Has(frameworkApiKey))
        {
            api = CommandLine::GetInstance().Get<String>(frameworkApiKey);
        }
        // Check Assets/Config/ProjectSettings.json project settings
        else if (String frameworkApi; ProjectSettings::GetInstance().TryGet(frameworkApiKey, frameworkApi))
        {
            api = frameworkApi;
        }
        // Default based on platform
        else
        {
#if IS_PLATFORM_WINDOWS
            api = FrameworkAPI::GLFW;
#endif
        }

        JPT_ASSERT(api != FrameworkAPI::Unknown, "No Framework API specified in CommandLine or ProjectSettings.json.");
        return api;
    }

    Framework* Framework_Create(FrameworkAPI api)
    {
        switch (api.Value())
        {
        case FrameworkAPI::GLFW:
            return new Framework_GLFW();

        case FrameworkAPI::Win32:
            return new Framework_Win32();

        default:
            JPT_ASSERT(false, "Un-implemented Framework API: %s", ToString(api).ConstBuffer());
            return nullptr;
        }
    }
}