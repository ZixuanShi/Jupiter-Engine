// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.System.Paths;

// Should be called in client's Application.cpp file once
#define JPT_SYNC_CLIENT(ApplicationName)                                             \
namespace jpt                                                                        \
{                                                                                    \
    Application* GetApplication()                                                    \
    {                                                                                \
        static Application_##ApplicationName app;                                    \
        return &app;                                                                 \
    }                                                                                \
                                                                                     \
    const char* Application::GetName() const { return #ApplicationName; }            \
                                                                                     \
    const wchar_t* System::Paths::GetClientDirW() const                              \
    {                                                                                \
        return JPT_CLIENT_DIR_W;                                                     \
    }                                                                                \
}

