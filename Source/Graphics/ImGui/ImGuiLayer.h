// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

// Import-free: a .mm includes it, and clang refuses a C++-built std.pcm in an ObjC++ TU.
namespace MTL  { class Device; }
namespace MTL4 { class CommandQueue; class CommandBuffer; class RenderCommandEncoder; class RenderPassDescriptor; }

namespace jpt
{
    // No-ops wherever IS_EDITOR is 0, so callers never guard. See ImGuiLayer.cpp.

    void ImGuiTerminate();

    // One frame stale: NewFrame() computes these, and it runs inside the renderer's BeginFrame().
    [[nodiscard]] bool ImGuiWantsMouse();
    [[nodiscard]] bool ImGuiWantsKeyboard();

    // SDL_Window*, borrowed. void* so this header stays free of SDL.
    void ImGuiInitPlatform(void* pSDLWindow);

    /** const SDL_Event*, unfiltered. ProcessEvent's return says whether ImGui consumed it, and
        acting on it here would drop the release half of a press that began outside a panel. */
    void ImGuiProcessEvent(const void* pSDLEvent);

    /** pIniPath may be null, which disables layout persistence; ImGui keeps the pointer rather
        than copying, so the implementation copies it. framesInFlight must match the renderer's
        ring -- the backend sizes its own buffers off it and would overwrite geometry the GPU is
        still reading. */
    bool ImGuiInit(MTL::Device* pDevice, MTL4::CommandQueue* pQueue, int framesInFlight, const char* pIniPath);

    // frameIndex must be the ring slot the renderer picked its command allocator with.
    void ImGuiBeginFrame(MTL4::RenderPassDescriptor* pPass, int frameIndex);
    void ImGuiEndFrame(MTL4::CommandBuffer* pCommandBuffer, MTL4::RenderCommandEncoder* pEncoder);
}
