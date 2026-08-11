// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.ApplicationBase;

namespace jpt
{
    /** The running App, as the engine sees it. Declared by the engine, defined by the active
        project, resolved by the linker -- so no engine TU ever names the project's type. */
    ApplicationBase& GetApp();

    /** From CMake, not from JPT_SYNC_APP, and that is forced: an immediate function must be
        *defined* in every TU that calls it, and a definition the project supplies elsewhere is
        exactly what the GetApp() seam provides. consteval and that seam cannot both hold. */
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
