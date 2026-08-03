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
        virtual void Terminate();
        virtual void Update();          // Game logic
        virtual void PostUpdate() {}    // Camera

    public:
        void Run();

    public:
        bool OnSurfaceReady(Renderer::SurfaceHandle surface);
        void OnResize(uint32 pixelWidth, uint32 pixelHeight);
        void OnFrame();
    };
}
