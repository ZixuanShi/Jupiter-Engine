// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/Window/Window.h"
#include "Graphics/Renderer.h"

export module jpt.Application;

import jpt.TypeDefs;
import jpt.FrameTimer;
import jpt.Input;
import jpt.Scene;

export namespace jpt
{
    /** Base class every executable (Editor, and each Projects/<Name> App) derives from. */
    class Application
    {
    private:
        FrameTimer m_frameTimer;
        Window m_window;
        Renderer m_renderer;
        Scene m_scene;
        Input m_input;
        Status m_status = Status::Pending;

    public:
        virtual ~Application() = default;

        virtual bool PreInit();
        virtual bool Init();
        virtual void Terminate();
        virtual void Update();          // Game logic
        virtual void PostUpdate() {}    // Camera
        virtual void DrawUI();          // Editor panels. Runs with the render pass open.

    public:
        void Run();

    public:
        bool OnSurfaceReady(Renderer::SurfaceHandle surface);
        void OnResize(uint32 pixelWidth, uint32 pixelHeight);
        void OnFrame();
    
    public:
        [[nodiscard]] const FrameTimer& GetFrameTimer() const noexcept { return m_frameTimer; }
        [[nodiscard]] Window& GetWindow()                     noexcept { return m_window; }
        [[nodiscard]] Renderer& GetRenderer()                 noexcept { return m_renderer; }
        [[nodiscard]] Scene& GetScene()                       noexcept { return m_scene; }
        [[nodiscard]] Input& GetInput()                       noexcept { return m_input; }
    };
}
