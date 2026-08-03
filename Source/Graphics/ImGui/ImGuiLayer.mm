// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "ImGuiLayer.h"

#if !IS_CONFIG_RELEASE && (IS_PLATFORM_MACOS || IS_PLATFORM_IOS)

#include <string>

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

#if IS_PLATFORM_MACOS
    #import <Cocoa/Cocoa.h>
    #include "imgui_impl_osx.h"
#else
    #import <UIKit/UIKit.h>
#endif

#include "imgui.h"
#include "imgui_impl_metal4.h"

namespace
{
    // ImGui stores the pointer rather than copying the string, so it has to outlive the context.
    std::string g_iniPath;

    // Borrowed, like the layer: the view is owned by the window hierarchy. Kept as void* rather
    // than a strong ObjC static so ARC cannot quietly extend its life past the window's.
    void* g_pNativeView = nullptr;

#if IS_PLATFORM_IOS
    // No AppKit backend on iOS, so the clock and the display metrics are ours to feed.
    double g_lastTime = 0.0;

    // Widgets sized for a mouse are unusable under a fingertip. Everything -- font, padding,
    // scrollbars -- scales by this.
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

    /** The half of the frame that is platform rather than renderer, and so is backend-agnostic. */
    void NewFramePlatform()
    {
#if IS_PLATFORM_MACOS
        ImGui_ImplOSX_NewFrame((__bridge NSView*)g_pNativeView);
#else
        UIView* pView = (__bridge UIView*)g_pNativeView;
        ImGuiIO& io = ImGui::GetIO();

        const CGSize points = pView.bounds.size;
        const float scale = static_cast<float>(pView.contentScaleFactor);
        io.DisplaySize = ImVec2(static_cast<float>(points.width), static_cast<float>(points.height));
        io.DisplayFramebufferScale = ImVec2(scale, scale);

        const double now = CACurrentMediaTime();
        io.DeltaTime = (g_lastTime > 0.0) ? static_cast<float>(now - g_lastTime) : 1.0f / 60.0f;
        g_lastTime = now;
#endif

        ImGui::NewFrame();
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
        NewFramePlatform();
    }

    void ImGuiEndFrame(MTL4::CommandBuffer* pCommandBuffer, MTL4::RenderCommandEncoder* pEncoder)
    {
        ImGui::Render();
        ImGui_ImplMetal4_RenderDrawData(ImGui::GetDrawData(),
                                        (__bridge id<MTL4CommandBuffer>)(void*)pCommandBuffer,
                                        (__bridge id<MTL4RenderCommandEncoder>)(void*)pEncoder);
    }

    void ImGuiInitPlatform(void* pNativeView)
    {
        g_pNativeView = pNativeView;

#if IS_PLATFORM_MACOS
        // Installs its own NSEvent monitors, so no engine input path is involved.
        ImGui_ImplOSX_Init((__bridge NSView*)pNativeView);
#else
        UIView* pView = (__bridge UIView*)pNativeView;
        const float scale = static_cast<float>(pView.contentScaleFactor);

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

    void ImGuiTerminate()
    {
#if IS_PLATFORM_MACOS
        ImGui_ImplOSX_Shutdown();
#endif
        ImGui_ImplMetal4_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiOnPointerMoved(float x, float y)
    {
        ImGui::GetIO().AddMousePosEvent(x, y);
    }

    void ImGuiOnPointerButton(bool isDown)
    {
        ImGui::GetIO().AddMouseButtonEvent(0, isDown);
    }
}

#else // Release, or a platform with no ImGui backend

namespace jpt
{
    bool ImGuiInit(MTL::Device*, MTL4::CommandQueue*, int, const char*)   { return true; }
    void ImGuiInitPlatform(void*)                                         {}
    void ImGuiTerminate()                                                 {}
    void ImGuiBeginFrame(MTL4::RenderPassDescriptor*, int)                {}
    void ImGuiEndFrame(MTL4::CommandBuffer*, MTL4::RenderCommandEncoder*) {}
    void ImGuiOnPointerMoved(float, float)                                {}
    void ImGuiOnPointerButton(bool)                                       {}
}

#endif
