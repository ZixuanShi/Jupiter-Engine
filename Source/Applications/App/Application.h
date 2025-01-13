// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Time.TypeDefs;

import jpt.Framework.Enums;
import jpt.Graphics.Enums;

namespace jpt
{
	class Framework;
	class Window;
	class WindowManager;
	class Renderer;
	class Platform;
	class SceneManager;

	/** Base abstract class for applications.
		It holds window, renderer, audio, collision managers, etc.*/
	class Application
	{
	protected:
		Platform*      m_pPlatform      = nullptr;
		Framework*     m_pFramework     = nullptr;
		WindowManager* m_pWindowManager = nullptr;
		Renderer*      m_pRenderer      = nullptr;
		SceneManager*  m_pSceneManager  = nullptr;

		Framework_API m_frameworkAPI = Framework_API::Unknown;
		Graphics_API m_graphicsAPI   = Graphics_API::Unknown;

		bool m_shouldShutdown = false;

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
		SceneManager*  GetSceneManager()  const { return m_pSceneManager;  }

		Framework_API GetFrameworkAPI() const { return m_frameworkAPI; }
		Graphics_API  GetGraphicsAPI()  const { return m_graphicsAPI;  }

		Window* GetMainWindow() const;
		const char* GetName() const;

		void SetPlatform(Platform* pPlatform) { m_pPlatform = pPlatform; }
		void SetShouldShutdown();
	};

	Application* GetApplication();
}