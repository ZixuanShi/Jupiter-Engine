// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Validation/Assert.h"

#if IS_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

module jpt.Debugger;

namespace jpt
{
    bool Debugger::PreInit()
    {
        bool success = true;

#if ASSERT_ENABLED
        success &= m_assertImpl.PreInit();
#endif

        return success;
    }

    bool Debugger::IsDebuggerPresent() const
    {
#if IS_PLATFORM_WINDOWS
        return ::IsDebuggerPresent();
#else
        return false;
#endif
    }
}