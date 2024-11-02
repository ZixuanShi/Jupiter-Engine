// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#if IS_PLATFORM_WIN64

#include "Applications/App/Application_Base.h"

using HINSTANCE = struct HINSTANCE__*; 

import jpt.TypeDefs;

namespace jpt
{
	class Application_Win64 : public Application_Base
	{
	private:
		using Super = Application_Base;

	protected:
		/** The handle to an instance or handle to a module.
		    The operating system uses this value to identify the executable or EXE when it's loaded in memory. 
		    Certain Windows functions need the instance handle, for example to load icons or bitmaps. */
		HINSTANCE m_hInstance = nullptr;

		/** A flag that indicates whether the main application window is minimized, maximized, or shown normally. */
		int32 m_nCmdShow = 0;

	public:
		virtual bool PreInit() override;

	public:
		void SetHINSTANCE(HINSTANCE hInstance) { m_hInstance = hInstance; }
		void SetnCmdShow(int32 nCmdShow) { m_nCmdShow = nCmdShow; }
	};

	Application_Win64* GetWin64Application();
}

#endif // IS_PLATFORM_WIN64