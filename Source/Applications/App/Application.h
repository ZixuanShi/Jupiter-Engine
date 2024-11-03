// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Time.TypeDefs;

namespace jpt
{
	class Window;
	class Framework;

	/** Base abstract class for applications.
		It holds window, renderer, audio, collision managers, etc.*/
	class Application
	{
	protected:
		Framework* m_pFramework = nullptr;
		Window* m_pMainWindow = nullptr;
		bool m_shouldShutdown = false;

	public:
		virtual ~Application() = default;

		virtual bool PreInit();
		virtual bool Init();
		virtual void Update(TimePrecision deltaSeconds);
		virtual void Shutdown();

		void Run();
		void ShutdownApp() { m_shouldShutdown = true; }

		Framework* GetFramework() const { return m_pFramework; }
		Window* GetMainWindow() const { return m_pMainWindow; }
		bool ShouldShutdown() const { return m_shouldShutdown; }

	protected:
		void ProcessInput();
	};

	Application* GetApplication();
}