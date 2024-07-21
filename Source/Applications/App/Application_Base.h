// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

import jpt.Framework_Base;
import jpt.Window_Base;
import jpt.Renderer_Base;

namespace jpt
{
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
		static Application_Base* GetInstance();

		virtual bool PreInit();
		virtual bool Init() { return true; }
		virtual void Update();
		virtual void Terminate();

		void Run();
		void TerminateApp();

	protected:
		void PollInput() { }
		void Render()    { }
	};
}