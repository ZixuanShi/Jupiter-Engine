// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.ApplicationBase;

namespace jpt
{
    /** The running App, as the engine sees it. Declared by the engine and defined by the active
        project -- a plain call, so the linker is what resolves it and no engine translation unit
        ever names the project's type. */
    ApplicationBase& GetApp();

    /** The active project's name: its directory's, which is also the binary's.

        From CMake, not from JPT_SYNC_APP, and that is forced. An immediate function must be
        *defined* in every translation unit that calls it -- constant evaluation runs in the
        compiler, and the linker has nothing to contribute to it. A declaration the project
        defines elsewhere is precisely what this seam exists to provide, so consteval and the
        seam cannot both hold. The name is build-time knowledge either way. */
    consteval const char* GetAppName() { return JUPITER_APP_NAME; }
}

/** Hands the engine the project's Application. Expanded once, at the bottom of Projects/<Name>/Source/Application<Name>.cpp */
#define JPT_SYNC_APP(AppClass)                  \
    namespace jpt                               \
    {                                           \
        ApplicationBase& GetApp()               \
        {                                       \
            static AppClass s_app;              \
            return s_app;                       \
        }                                       \
    }
