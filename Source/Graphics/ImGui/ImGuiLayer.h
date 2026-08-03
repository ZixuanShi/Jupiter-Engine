// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

// Import-free: ImGuiLayer.mm includes it, and clang refuses to load a C++-built std.pcm into an
// Objective-C++ translation unit. Same rule that governs AppleCallbacks.h.
//
// metal-cpp types are forward-declared rather than passed as void*, which keeps the renderer's
// calls type-checked. The .mm bridges them to their Objective-C selves, which is all they ever
// were -- metal-cpp is a typed view over objc_msgSend.
namespace MTL { class Device; class CommandBuffer; class RenderCommandEncoder; class RenderPassDescriptor; }

namespace jpt
{
    /** Every function here is a no-op in Release, where ImGui is not linked at all. Callers do
        not guard, which is what keeps #if out of the renderer and the window classes. */

    /** pIniPath may be null, which disables layout persistence. ImGui keeps the pointer rather
        than copying, so this copies it. */
    bool ImGuiInit(MTL::Device* pDevice, const char* pIniPath);
    void ImGuiTerminate();

    /** The native view: NSView* on macOS, UIView* on iOS. void* because only a .mm can name it,
        and this header is compiled as C++ too. Held borrowed for the frame calls below, which
        is why the renderer never has to know the view exists. */
    void ImGuiInitPlatform(void* pNativeView);

    void ImGuiBeginFrame(MTL::RenderPassDescriptor* pPass);
    void ImGuiEndFrame(MTL::CommandBuffer* pCommandBuffer, MTL::RenderCommandEncoder* pEncoder);

    /** iOS only -- macOS input arrives through imgui_impl_osx's own event monitors. Coordinates
        are in points, which is what ImGui works in; DisplayFramebufferScale covers the rest. */
    void ImGuiOnPointerMoved(float x, float y);
    void ImGuiOnPointerButton(bool isDown);
}
