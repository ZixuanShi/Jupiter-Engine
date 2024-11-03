// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Time.TypeDefs;

import jpt.Framework.Enums;
import jpt.Graphics.Enums;

namespace jpt
{
	class Window;
	class Framework;
	class Renderer;

	/** Base abstract class for applications.
		It holds window, renderer, audio, collision managers, etc.*/
	class Application
	{
	protected:
		Framework* m_pFramework = nullptr;
		Window* m_pMainWindow = nullptr;
		Renderer* m_pRenderer = nullptr;
		bool m_shouldShutdown = false;

		Framework_API m_frameworkAPI = Framework_API::Unknown;
		Graphics_API m_graphicsAPI = Graphics_API::Unknown;

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
		const char* GetName() const;

	protected:
		void ProcessInput();
	};

	Application* GetApplication();
}