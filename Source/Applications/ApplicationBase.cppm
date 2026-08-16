// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Graphics/Renderer.h"

export module jpt.ApplicationBase;

import jpt.Window;
import jpt.TypeDefs;
import jpt.FrameTimer;
import jpt.Input;
import jpt.Scene;
import jpt.LaunchArgs;

#if IS_EDITOR
    import jpt.EditorUI;
#endif

export namespace jpt
{
    /** The engine half of the application, shared by every Projects/<Name> App.

        The one polymorphic type in the engine, and the only sanctioned `virtual` -- everything
        below it is compile-time polymorphism a static_assert enforces. The exception is what buys
        the Engine/App split its simplicity: engine code calls through ApplicationBase& and lands
        on the App's overrides, so it never has to name a type the app target owns. One vtable in
        one process-lifetime object is the whole cost. */
    class ApplicationBase
    {
    private:
        FrameTimer m_frameTimer;
        Window m_window;
        Renderer m_renderer;
        Scene m_scene;
        Input m_input;
        LaunchArgs m_launchArgs;
        Status m_status = Status::Pending;

#if IS_EDITOR
        EditorUI m_editorUI;
#endif

    public:
        virtual ~ApplicationBase() = default;

        virtual bool PreInit(int argc, char* argv[]);
        virtual bool Init();
        virtual void Update();          // Game logic
        virtual void PostUpdate();      // Camera
        virtual void Terminate();

    public:
        void OnFrame();

        // Virtual because it is where content is uploaded, and content is the App's. On iOS the
        // surface arrives long after Init(), so there is no earlier hook a project could use.
        virtual bool OnSurfaceReady(Renderer::SurfaceHandle surface);
        void OnResize(uint32 pixelWidth, uint32 pixelHeight);

        /** iOS background/foreground. Toggles the Running/Paused pair only, so a foreground event
            arriving after SDL_AppQuit cannot resurrect a terminated app. */
        void SetPaused(bool paused) noexcept;

    public:
        [[nodiscard]] const FrameTimer& GetFrameTimer() const noexcept { return m_frameTimer; }
        [[nodiscard]] Window& GetWindow()                     noexcept { return m_window; }
        [[nodiscard]] Renderer& GetRenderer()                 noexcept { return m_renderer; }
        [[nodiscard]] Scene& GetScene()                       noexcept { return m_scene; }
        [[nodiscard]] Input& GetInput()                       noexcept { return m_input; }
        [[nodiscard]] const Input& GetInput() const           noexcept { return m_input; }
        [[nodiscard]] const LaunchArgs& GetLaunchArgs() const noexcept { return m_launchArgs; }
    };
}
