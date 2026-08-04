// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

// Import-free: a .mm includes it, and clang refuses a C++-built std.pcm in an ObjC++ TU.
// metal-cpp types are forward-declared rather than void*, so the renderer's calls stay checked.
namespace MTL  { class Device; }
namespace MTL4 { class CommandQueue; class CommandBuffer; class RenderCommandEncoder; class RenderPassDescriptor; }

namespace jpt
{
    // No-ops in Release, where ImGui is not linked. Callers never guard.

    void ImGuiTerminate();

    /** True when the UI is using the device and the engine should drop the event. One frame
        stale: NewFrame() computes them, and that runs inside the renderer's BeginFrame(). */
    [[nodiscard]] bool ImGuiWantsMouse();
    [[nodiscard]] bool ImGuiWantsKeyboard();

    // NSView* on macOS, UIView* on iOS. void* because only a .mm can name it. Borrowed.
    void ImGuiInitPlatform(void* pNativeView);

    /** iOS only -- macOS input arrives through imgui_impl_osx's own event monitors. Coordinates
        are in points, which is what ImGui works in; DisplayFramebufferScale covers the rest. */
    void ImGuiOnPointerMoved(float x, float y);
    void ImGuiOnPointerButton(bool isDown);

    /** pIniPath may be null, which disables layout persistence. ImGui keeps the pointer rather
        than copying, so the implementation copies it.

        The Metal 4 backend needs the queue and framesInFlight up front -- it sizes its own vertex,
        index and constant buffers off them, so framesInFlight must match the renderer's ring or it
        will overwrite geometry the GPU is still reading. */
    bool ImGuiInit(MTL::Device* pDevice, MTL4::CommandQueue* pQueue, int framesInFlight, const char* pIniPath);

    /** frameIndex is the renderer's own ring slot, and must be the one it used to pick the
        command allocator -- that is the whole basis of the backend's buffer reuse. */
    void ImGuiBeginFrame(MTL4::RenderPassDescriptor* pPass, int frameIndex);
    void ImGuiEndFrame(MTL4::CommandBuffer* pCommandBuffer, MTL4::RenderCommandEncoder* pEncoder);
}
