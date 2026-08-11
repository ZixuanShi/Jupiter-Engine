// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "ImGuiLayer.h"

#if IS_EDITOR

#include <string>

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

#include <SDL3/SDL.h>

#include "imgui.h"
#include "imgui_impl_metal4.h"
#include "imgui_impl_sdl3.h"

namespace
{
    // ImGui stores the pointer rather than copying the string, so it has to outlive the context.
    std::string g_iniPath;

// Form factor rather than platform: widgets sized for a mouse are unusable under a fingertip,
// whichever OS the finger belongs to. Everything -- font, padding, scrollbars -- scales by this.
#if IS_MOBILE
    constexpr float kTouchScale = 2.0f;
#endif

    /** Everything the two backends share, which is all of it up to the backend's own init. */
    void CreateContext(const char* pIniPath)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        g_iniPath = pIniPath ? pIniPath : "";
        io.IniFilename = g_iniPath.empty() ? nullptr : g_iniPath.c_str();
    }

}

namespace jpt
{
    bool ImGuiInit(MTL::Device* pDevice, MTL4::CommandQueue* pQueue, int framesInFlight, const char* pIniPath)
    {
        CreateContext(pIniPath);

        // The same objects seen through two type systems: metal-cpp is a typed view over
        // objc_msgSend, so the bridge is a cast and nothing else.
        return ImGui_ImplMetal4_Init((__bridge id<MTLDevice>)(void*)pDevice,
                                     (__bridge id<MTL4CommandQueue>)(void*)pQueue,
                                     framesInFlight);
    }

    void ImGuiBeginFrame(MTL4::RenderPassDescriptor* pPass, int frameIndex)
    {
        // Needs the live descriptor: the backend reads the attachment formats off the attached
        // textures, so a format-only descriptor would give it MTLPixelFormatInvalid.
        ImGui_ImplMetal4_NewFrame((__bridge MTL4RenderPassDescriptor*)(void*)pPass, frameIndex);
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiEndFrame(MTL4::CommandBuffer* pCommandBuffer, MTL4::RenderCommandEncoder* pEncoder)
    {
        ImGui::Render();
        ImGui_ImplMetal4_RenderDrawData(ImGui::GetDrawData(),
                                        (__bridge id<MTL4CommandBuffer>)(void*)pCommandBuffer,
                                        (__bridge id<MTL4RenderCommandEncoder>)(void*)pEncoder);
    }

    void ImGuiInitPlatform(void* pSDLWindow)
    {
        SDL_Window* pWindow = static_cast<SDL_Window*>(pSDLWindow);
        ImGui_ImplSDL3_InitForMetal(pWindow);

#if IS_MOBILE
        const float scale = SDL_GetWindowPixelDensity(pWindow);
        ImGuiIO& io = ImGui::GetIO();

        // Rasterize at scale so the glyphs are sharp, then divide it back out so they occupy
        // the point size we asked for. Without the second half, text is scale-times too big.
        ImFontConfig fontConfig;
        fontConfig.SizePixels = 13.0f * kTouchScale * scale;
        io.Fonts->AddFontDefault(&fontConfig);
        io.FontGlobalScale = 1.0f / scale;

        ImGui::GetStyle().ScaleAllSizes(kTouchScale);
#endif
    }

    void ImGuiProcessEvent(const void* pSDLEvent)
    {
        // InputTests drives Window::OnEvent() from PreInit, long before the context exists.
        if (ImGui::GetCurrentContext() == nullptr)
        {
            return;
        }

        ImGui_ImplSDL3_ProcessEvent(static_cast<const SDL_Event*>(pSDLEvent));
    }

    void ImGuiTerminate()
    {
        if (ImGui::GetCurrentContext() == nullptr)
        {
            return;
        }

        ImGui_ImplSDL3_Shutdown();
        ImGui_ImplMetal4_Shutdown();
        ImGui::DestroyContext();
    }

    bool ImGuiWantsMouse()
    {
        return ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().WantCaptureMouse;
    }

    bool ImGuiWantsKeyboard()
    {
        return ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().WantCaptureKeyboard;
    }
}

#endif // IS_EDITOR
