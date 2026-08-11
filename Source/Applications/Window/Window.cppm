// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <SDL3/SDL.h>

// In the global module fragment on purpose: RendererMetal4.h forward-declares the same name, and
// a declaration inside the module purview would attach to jpt.Window and be a different type.
#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
    namespace CA { class MetalLayer; }
#endif

export module jpt.Window;

import jpt.TypeDefs;

export namespace jpt
{
    /** SDL3 on every platform, so only the surface differs by backend. */
    class Window
    {
    public:
#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS
        using SurfaceHandle = CA::MetalLayer*;
#else
        using SurfaceHandle = void*;
#endif

    private:
        SDL_Window*   m_pWindow    = nullptr;
        SDL_MetalView m_metalView  = nullptr;
        SurfaceHandle m_surface    = nullptr;    // Borrowed from the view

        uint32 m_width  = 0;
        uint32 m_height = 0;

        // Cached, so translation works with no window: InputTests drives OnEvent before SDL_Init.
        float32 m_pixelDensity = 1.0f;

        // Accumulated while captured: relative mode stops updating the event's position, and
        // Input differences positions rather than taking deltas.
        float32 m_reportedX = 0.0f;
        float32 m_reportedY = 0.0f;

#if IS_PC
        // Where capture began, so releasing puts the pointer back rather than where it wandered to.
        float32 m_anchorX = 0.0f;
        float32 m_anchorY = 0.0f;
#endif

        bool m_cursorCaptured = false;

    public:
        bool PreInit();
        bool Init();
        void Terminate();

    public:
        /** Running continues the app, Succeeded quits cleanly, Failed quits with an error. */
        [[nodiscard]] Status OnEvent(const SDL_Event& event);

        void OnResize(uint32 pixelWidth, uint32 pixelHeight) noexcept;

        // Hides the pointer and feeds relative motion. Driven by the transition, not the state:
        // the anchor is taken once.
        void SetCursorCaptured(bool captured);

        [[nodiscard]] SurfaceHandle GetSurface() const noexcept { return m_surface; }
        [[nodiscard]] void* GetNativeHandle() const noexcept { return m_pWindow; }
        [[nodiscard]] uint32 GetWidth()  const noexcept { return m_width; }
        [[nodiscard]] uint32 GetHeight() const noexcept { return m_height; }
        [[nodiscard]] float32 GetAspectRatio() const noexcept;
    };
}
