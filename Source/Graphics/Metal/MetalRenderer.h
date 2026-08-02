// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <cstdint>

namespace jpt
{
    /** Takes a CAMetalLayer* as an opaque handle so this header stays free of ObjC types
        and can be included from plain C++ translation units. */
    bool InitRenderer(void* metalLayer);

    /** Tells the renderer the drawable resized. Sizes are in pixels, not points. */
    void ResizeRenderer(std::uint32_t pixelWidth, std::uint32_t pixelHeight);

    /** Renders and presents one frame. elapsedSeconds drives the animation. */
    void DrawFrame(double elapsedSeconds);

    void ShutdownRenderer();
}
