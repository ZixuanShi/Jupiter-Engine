// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

module jpt.FilePathUtils;

import jpt.Debugger;
import jpt.SystemPaths;

namespace jpt::File
{
    Path Combine(Source source, const Path& relativePath)
    {
        Path path;
        System::Paths& paths = System::Paths::GetInstance();

        switch (source)
        {
        case Source::Engine:
            path.Append(paths.GetEngineDir());
            break;
        case Source::Client:
            path.Append(paths.GetClientDir());
            break;
        case Source::Output:
            path.Append(paths.GetOutputDir());
            break;
        case Source::Saved:
            path.Append(paths.GetSavedDir());
            break;

        case Source::Baked:
#if IS_CONFIG_RELEASE
            path.Append(paths.GetOutputDir());
#else
            if (relativePath.Has("Jupiter_Common"))
            {
                path.Append(paths.GetEngineDir());
            }
            else
            {
                path.Append(paths.GetClientDir());
            }
#endif
            path.Append(L"_Baked/");

            break;

        default:
            JPT_ASSERT(false, "Unknown source");
        }

        path.Append(relativePath);

        return path;
    }

    Path FixDependencies(const Path& relativePath)
    {
#if IS_CONFIG_RELEASE
        // If ran from VS debugger, use absolute path to Client as it's using Visual Studio's working directory
        if (Debugger::GetInstance().IsDebuggerPresent())
        {
            if (relativePath.Has("Jupiter_Common"))
            {
                return Combine(Source::Engine, relativePath);
            }
            else
            {
                return Combine(Source::Client, relativePath);
            }
        }
        // Release config will have copied assets to the output directory
        else
        {
            return relativePath;
        }
#else
        if (relativePath.Has("Jupiter_Common"))
        {
            return Combine(Source::Engine, relativePath);
        }
        else
        {
            return Combine(Source::Client, relativePath);
        }
#endif
    }
}