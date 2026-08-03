// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#include <cstdint>

// A forward declaration is all this needs, so the .mm files stay free of metal-cpp. On Apple
// the renderer is always Metal, so naming the type here costs no portability.
namespace CA { class MetalLayer; }

namespace jpt
{
    // Platform -> app
    bool OnSurfaceReady(CA::MetalLayer* pMetalLayer);
    void OnResize(std::uint32_t pixelWidth, std::uint32_t pixelHeight);
    void OnFrame();
    void OnTerminate();
}
