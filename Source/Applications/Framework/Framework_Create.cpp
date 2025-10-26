// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Memory/Memory.h"
#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

module jpt.Framework_Create;

import jpt.Framework_GLFW;
import jpt.Framework_Win32;

import jpt.Settings;

namespace jpt
{
    FrameworkAPI FindFrameworkAPI()
    {
        FrameworkAPI api = FrameworkAPI::Unknown;
        const String frameworkApiKey = "frameworkAPI";

#if IS_PLATFORM_WINDOWS
        api = SyncSettings(frameworkApiKey, FrameworkAPI::GLFW);
#endif

        JPT_ASSERT(api != FrameworkAPI::Unknown, "No Framework API specified in CommandLine or ProjectSettings.json.");
        return api;
    }

    Framework* Framework_Create()
    {
        const FrameworkAPI api = FindFrameworkAPI();

        switch (api.Value())
        {
        case FrameworkAPI::GLFW:
            return JPT_NEW(Framework_GLFW);

        case FrameworkAPI::Win32:
            return JPT_NEW(Framework_Win32);

        default:
            JPT_ASSERT(false, "Un-implemented Framework API: %s", ToString(api).ConstBuffer());
            return nullptr;
        }
    }
}