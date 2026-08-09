// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.ApplicationBase;

namespace jpt
{
    /** The running App, as the engine sees it. Declared by the engine and defined by the active project */
    ApplicationBase& GetApp();
}

/** Hands the engine the project's Application. Expanded once, at the bottom of Projects/<Name>/Source/Application<Name>.cpp */
#define JPT_SYNC_APP(AppClass)          \
    namespace jpt                       \
    {                                   \
        ApplicationBase& GetApp()       \
        {                               \
            static AppClass s_app;      \
            return s_app;               \
        }                               \
    }
