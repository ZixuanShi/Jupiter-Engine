// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/Window/Window.h"
#include "Graphics/Renderer.h"

export module jpt.Application;

import jpt.TypeDefs;
import jpt.FrameTimer;

export namespace jpt
{
    /** Base class every executable (Editor, and each Projects/<Name> App) derives from. */
    class Application
    {
    private:
        FrameTimer m_frameTimer;
        Window m_window;
        Renderer m_renderer;
        Status m_status = Status::Pending;

    public:
        virtual ~Application() = default;

        virtual bool PreInit();
        virtual bool Init();
        virtual void Update();
        virtual void Terminate();

    public:
        void Run();

    public:
        bool OnSurfaceReady(Renderer::SurfaceHandle surface);
        void OnResize(uint32 pixelWidth, uint32 pixelHeight);
        void OnFrame();
    
    public:
        // m_frameTimer is private, so without this an override of Update() has no delta time.
        [[nodiscard]] const FrameTimer& GetFrameTimer() const noexcept { return m_frameTimer; }

        [[nodiscard]] Window& GetWindow() noexcept { return m_window; }
        [[nodiscard]] const Window& GetWindow() const noexcept { return m_window; }
        [[nodiscard]] Renderer& GetRenderer() noexcept { return m_renderer; }
        [[nodiscard]] const Renderer& GetRenderer() const noexcept { return m_renderer; }
    };
}
