// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <cstdint>

namespace jpt
{
    // --- App -> platform ---
    bool CreateAppWindow(std::int32_t width, std::int32_t height, const char* title);
    void RunAppLoop(int argc, char* argv[]);

    // --- Platform -> app ---
    // std:: rather than jpt's aliases: the .mm files include this, and jpt.TypeDefs imports
    // std, which clang refuses to load into an Objective-C++ translation unit.
    bool OnSurfaceReady(void* pMetalLayer);
    void OnResize(std::uint32_t pixelWidth, std::uint32_t pixelHeight);
    void OnFrameDraw();
    void OnTerminate();
}
