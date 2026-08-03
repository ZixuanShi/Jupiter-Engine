// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS

#include "AppleCallbacks.h"
#include "Applications/AppClient.h"

namespace jpt
{
    bool OnSurfaceReady(CA::MetalLayer* pMetalLayer)
    {
        return GetApplication().OnSurfaceReady(pMetalLayer);
    }

    void OnResize(std::uint32_t pixelWidth, std::uint32_t pixelHeight)
    {
        GetApplication().OnResize(pixelWidth, pixelHeight);
    }

    void OnFrame()
    {
        GetApplication().OnFrame();
    }

    void OnTerminate()
    {
        GetApplication().Terminate();
    }
}

#endif // IS_PLATFORM_MACOS || IS_PLATFORM_IOS
