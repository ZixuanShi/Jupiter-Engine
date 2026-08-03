// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

// Import-free: ImGuiLayer.mm includes it, and clang refuses to load a C++-built std.pcm into an
// Objective-C++ translation unit. Same rule that governs AppleCallbacks.h.
//
// metal-cpp types are forward-declared rather than passed as void*, which keeps the renderer's
// calls type-checked. The .mm bridges them to their Objective-C selves, which is all they ever
// were -- metal-cpp is a typed view over objc_msgSend.
namespace MTL  { class Device; }
namespace MTL4 { class CommandQueue; class CommandBuffer; class RenderCommandEncoder; class RenderPassDescriptor; }

namespace jpt
{
    /** Every function here is a no-op in Release, where ImGui is not linked at all. Callers do
        not guard, which is what keeps #if out of the renderer and the window classes. */

    void ImGuiTerminate();

    /** The native view: NSView* on macOS, UIView* on iOS. void* because only a .mm can name it,
        and this header is compiled as C++ too. Held borrowed for the frame calls below, which
        is why the renderer never has to know the view exists. */
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
