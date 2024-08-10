// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Time.TypeDefs;

namespace jpt
{
	class Window_Base;
	class Framework_Base;
	class Renderer_Base;

	/** Base abstract class for applications.
		It holds window, renderer, audio, collision managers, etc.*/
	class Application_Base
	{
	protected:
		Framework_Base* m_pFramework    = nullptr;
		Window_Base*    m_pWindow       = nullptr;
		Renderer_Base*  m_pRenderer     = nullptr;

		bool m_shouldShutdown = false;

	public:
		virtual ~Application_Base() = default;

		virtual bool PreInit();
		virtual bool Init();
		virtual void Update(TimePrecision deltaSeconds);
		virtual void Shutdown();

		void Run();
		void ShutdownApp() { m_shouldShutdown = true; }

		Window_Base*    GetWindow()    const { return m_pWindow;    }
		Framework_Base* GetFramework() const { return m_pFramework; }
		Renderer_Base*  GetRenderer()  const { return m_pRenderer;  }

	protected:
		void ProcessInput();
		void Render();
	};
}