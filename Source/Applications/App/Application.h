// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Time.TypeDefs;

namespace jpt
{
	class Window;
	class Framework;
	class Renderer_Base;

	/** Base abstract class for applications.
		It holds window, renderer, audio, collision managers, etc.*/
	class Application
	{
	protected:
		Framework* m_pFramework = nullptr;
		Window* m_pWindow = nullptr;
		//Renderer_Base*  m_pRenderer     = nullptr;

		bool m_shouldShutdown = false;

	public:
		virtual ~Application() = default;

		virtual bool PreInit();
		virtual bool Init();
		virtual void Update(TimePrecision deltaSeconds);
		virtual void Shutdown();

		void Run();
		void ShutdownApp() { m_shouldShutdown = true; }

		Window* GetWindow()    const { return m_pWindow; }
		Framework* GetFramework() const { return m_pFramework; }
		//Renderer_Base*  GetRenderer()  const { return m_pRenderer;  }
		bool ShouldShutdown() const { return m_shouldShutdown; }

	protected:
		void ProcessInput();
		void Render();
	};

	Application* GetApplication();
}