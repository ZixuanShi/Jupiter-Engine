// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

module jpt.Platform.Win64;

#if IS_PLATFORM_WIN64

namespace jpt
{
    bool Platform_Win64::PreInit()
    {
        JPT_ENSURE(Super::PreInit());
        JPT_ENSURE(m_hInstance != nullptr);

        return true;
    }
}

#endif // IS_PLATFORM_WIN64