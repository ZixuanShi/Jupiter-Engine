// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Application;

import jpt.TypeDefs;
import jpt.Status;
import jpt.Framework.Enums;
import jpt.Graphics.Enums;

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

        FrameworkAPI m_frameworkAPI = FrameworkAPI::Unknown;
        GraphicsAPI m_graphicsAPI   = GraphicsAPI::Unknown;

        Status m_status = Status::Pending;

    public:
        virtual ~Application() = default;

        virtual bool PreInit();
        virtual bool Init();
        virtual void Update(TimePrecision deltaSeconds);
        virtual void Shutdown();

    public:
        void Run();

    public:
        Platform*      GetPlatform()      const { return m_pPlatform;      }
        Framework*     GetFramework()     const { return m_pFramework;     }
        WindowManager* GetWindowManager() const { return m_pWindowManager; }
        Renderer*      GetRenderer()      const { return m_pRenderer;      }

        FrameworkAPI GetFrameworkAPI() const { return m_frameworkAPI; }
        GraphicsAPI  GetGraphicsAPI()  const { return m_graphicsAPI;  }

        Window* GetMainWindow() const;
        const char* GetName() const;

        void SetPlatform(Platform* pPlatform) { m_pPlatform = pPlatform; }
        void SetStatus(Status status);
    };

    Application* GetApplication();
}