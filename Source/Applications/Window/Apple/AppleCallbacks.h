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

#if IS_PLATFORM_MACOS
    void OnKeyDown(std::uint16_t platformKeyCode, bool isRepeat);
    void OnKeyUp(std::uint16_t platformKeyCode);

    // macOS emits no down/up for modifiers, only the whole mask.
    void OnModifierChanged(std::uint16_t platformKeyCode, std::uint32_t modifierFlags);

    void OnMouseButton(std::int32_t buttonNumber, bool isDown, float x, float y);
    void OnMouseMove(float x, float y);
    void OnMouseScroll(float deltaX, float deltaY, bool isPrecise);
#endif

    // touchId is a stable handle for the finger, minted by the platform layer. timeSeconds is the
    // event's own timestamp, not the engine clock.
    // Both platforms: iOS from the screen, macOS from the trackpad. 
    void OnTouchBegan(std::uint64_t touchId, float x, float y, double timeSeconds);
    void OnTouchMoved(std::uint64_t touchId, float x, float y, double timeSeconds);
    void OnTouchEnded(std::uint64_t touchId, float x, float y, double timeSeconds);
    void OnTouchCancelled(std::uint64_t touchId, float x, float y, double timeSeconds);
}
