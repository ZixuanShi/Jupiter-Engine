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
		Window_Base* m_pWindow = nullptr;
		Framework_Base* m_pFramework = nullptr;
		bool m_shouldTerminate = false;

	public:
		virtual ~Application_Base() = default;

		virtual bool PreInit();
		virtual bool Init();
		virtual void Update(TimePrecision deltaSeconds);
		virtual void Terminate();

		void Run();
		void TerminateApp();

	protected:
		void ProcessInput() { }
		void Render()    { }
	};
}