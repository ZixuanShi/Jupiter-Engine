// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"
#include "Debugging/Logger.h"

#if IS_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

module jpt.AssertImpl;

import jpt.String;

namespace jpt
{
#if ASSERT_ENABLED
    void locAssertCallback(int line, const char* file, const char* expression, const char* message)
    {
        String messageStr(expression);
        if (message)
        {
            messageStr += " - ";
            messageStr += message;
        }

        Logger::GetInstance().Log(Logger::Type::Error, line, file, messageStr.ConstBuffer());

#if IS_PLATFORM_WINDOWS
        MessageBoxA(nullptr, messageStr.ConstBuffer(), "Assertion Failed", MB_ABORTRETRYIGNORE);
#endif
    }
#endif

    bool AssertImpl::PreInit()
    {
#if ASSERT_ENABLED
        g_AssertCallback = locAssertCallback;
#endif
        return true;
    }
}