// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

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
        Renderer m_renderer;
        Status m_status = Status::Pending;

    public:
        virtual ~Application() = default;

        virtual bool PreInit();
        virtual bool Init();
        virtual void Update(float64 deltaSeconds);
        virtual void Terminate();

    public:
        void Run(int argc, char* argv[]);

    public:
        bool OnSurfaceReady(void* pMetalLayer);
        void OnResize(uint32 pixelWidth, uint32 pixelHeight);
        void OnFrameDraw();

    public:
        Renderer& GetRenderer() { return m_renderer; }
    };
}
