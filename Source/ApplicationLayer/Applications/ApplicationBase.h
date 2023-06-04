// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	class WindowBase;
	class RendererBase;

	/* Base abstract class for applications.
	   It holds window, renderer, audio, collision managers, etc.*/
	class JPT_API ApplicationBase
	{
	protected:
		WindowBase* m_pWindow;
		RendererBase* m_pRenderer;
		bool m_shouldQuit;

	public:
		ApplicationBase();
		virtual ~ApplicationBase() = default;

		virtual bool PreInit() { return true; }
		virtual bool Init();
		virtual void RunGameLoop();
		virtual void Clean();

	protected:
		virtual void Render();
		virtual void Update();
	};
}