// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS

#include "AppleWindow.h"
#include "Applications/AppClient.h"

// A plain translation unit, not part of jpt.Application: these are declared in a header the
// .mm files include, so they belong to the global module. Defining them inside a module's
// purview is ill-formed -- "declaration in module jpt.Application follows declaration in the
// global module" -- because the definition would attach to the module instead.
namespace jpt
{
    bool OnSurfaceReady(void* pMetalLayer)
    {
        return GetApplication().OnSurfaceReady(pMetalLayer);
    }

    void OnResize(std::uint32_t pixelWidth, std::uint32_t pixelHeight)
    {
        GetApplication().OnResize(pixelWidth, pixelHeight);
    }

    void OnFrameDraw()
    {
        GetApplication().OnFrameDraw();
    }

    void OnTerminate()
    {
        GetApplication().Terminate();
    }
}

#endif // IS_PLATFORM_MACOS || IS_PLATFORM_IOS
