// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

module jpt.Platform.Windows;

#if IS_PLATFORM_WINDOWS

namespace jpt
{
    Platform_Windows::Platform_Windows(HINSTANCE hInstance, int nCmdShow)
        : m_hInstance(hInstance)
        , m_nCmdShow(nCmdShow)
    {
    }

    bool Platform_Windows::PreInit()
    {
        JPT_ENSURE(Super::PreInit());
        JPT_ENSURE(m_hInstance != nullptr);

        return true;
    }
}

#endif // IS_PLATFORM_WINDOWS