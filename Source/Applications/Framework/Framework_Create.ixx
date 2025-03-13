// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

export module jpt.Framework.Create;

import jpt.Framework;
import jpt.Framework.Enums;
import jpt.Framework_GLFW;
import jpt.Framework_Win32;

import jpt.CommandLine;
import jpt.ProjectSettings;

export namespace jpt
{
    Framework_API FindFrameworkAPI()
    {
        Framework_API api = Framework_API::Unknown;

        // Check CommandLine for framework_api
        if (CommandLine::GetInstance().Has("framework_api"))
        {
            api = CommandLine::GetInstance().Get<String>("framework_api");
        }
        // Check Assets/Config/ProjectSettings.json project settings
        else if (String frameworkApi; ProjectSettings::GetInstance().TryGet("framework_api", frameworkApi))
        {
            api = frameworkApi;
        }
        // Default based on platform
        else
        {
#if IS_PLATFORM_WIN64
            api = Framework_API::GLFW;
#endif
        }

        JPT_ASSERT(api != Framework_API::Unknown, "No Framework API specified in CommandLine or ProjectSettings.json.");
        return api;
    }

    Framework* Framework_Create(Framework_API api)
    {
        switch (api.Value())
        {
        case Framework_API::GLFW:
            return new Framework_GLFW();

        case Framework_API::Win32:
            return new Framework_Win32();

        default:
            JPT_ASSERT(false, "Un-implemented Framework API: %s", ToString(api).ConstBuffer());
            return nullptr;
        }
    }
}