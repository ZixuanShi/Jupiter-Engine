// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	class WindowBase;
	class RendererBase;

	/** Base abstract class for applications.
	    It holds window, renderer, audio, collision managers, etc.*/
	class JPT_API ApplicationBase
	{
	protected:
		WindowBase* m_pWindow = nullptr;
		RendererBase* m_pRenderer = nullptr;
		bool m_shouldQuit = false;

	public:
		virtual ~ApplicationBase() = default;

		virtual bool PreInit();
		virtual bool Init();
		virtual void RunGameLoop();
		virtual void Clean();

	protected:
		virtual void Render();
		virtual void Update();
	};
}