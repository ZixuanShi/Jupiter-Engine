// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Application;

import jpt.TypeDefs;
import jpt.Status;

export namespace jpt
{
    class Framework;
    class Window;
    class WindowManager;
    class Renderer;
    class Platform;
    class SceneManager;
    class AssetManager;

    /** Base abstract class for applications.
        It holds window, renderer, audio, collision managers, etc.*/
    class Application
    {
    protected:
        Platform*      m_pPlatform      = nullptr;
        Framework*     m_pFramework     = nullptr;
        WindowManager* m_pWindowManager = nullptr;
        Renderer*      m_pRenderer      = nullptr;

        Status m_status = Status::Pending;

    public:
        virtual ~Application() = default;

        virtual bool PreInit();
        virtual bool Init();
        virtual void Update(TimePrecision deltaSeconds);
        virtual void Terminate();

    public:
        void Run();

    public:
        Platform*      GetPlatform()      const { return m_pPlatform;      }
        Framework*     GetFramework()     const { return m_pFramework;     }
        WindowManager* GetWindowManager() const { return m_pWindowManager; }
        Renderer*      GetRenderer()      const { return m_pRenderer;      }

        Window* GetMainWindow() const;
        const char* GetName() const;

        void SetPlatform(Platform* pPlatform) { m_pPlatform = pPlatform; }
        void SetStatus(Status status) { m_status = status; }
    };

    Application* GetApplication();
}