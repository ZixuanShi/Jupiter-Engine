// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Assert.h"

#if ASSERT_ENABLED

#include "Core/Strings/StringMacros.h"

#if IS_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

namespace jpt
{
    void OnAssertionFailed(int line, const char* file, const char* expression, const char* format, ...)
    {
        if (!g_AssertCallback)
        {
            assert(false);
            return;
        }

        if (format)
        {
            char messageBuffer[512];
            JPT_FORMAT_STRING(messageBuffer, format, ...);
            g_AssertCallback(line, file, expression, messageBuffer);
        }
        else
        {
            g_AssertCallback(line, file, expression, nullptr);
        }
    }

    void OnAssertionFailed(int line, const char* file, const char* expression)
    {
        OnAssertionFailed(line, file, expression, nullptr);
    }
}
#endif // ASSERT_ENABLED