// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_WIN64

#include "Applications/App/Application_Base.h"
#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <Windows.h>

#endif;

export module jpt.Application_Win64;

#if IS_PLATFORM_WIN64

import jpt.TypeDefs;
import jpt.CommandLine;
import jpt.Utilities;
import jpt.Framework_GLFW;
import jpt.Window_GLFW;

namespace jpt
{
	export class Application_Win64 : public Application_Base
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
		virtual bool Init() override;

	public:
		void SetHINSTANCE(HINSTANCE hInstance) { m_hInstance = hInstance; }
		void SetnCmdShow(int32 nCmdShow) { m_nCmdShow = nCmdShow; }
	};

	bool Application_Win64::PreInit()
	{
		JPT_ENSURE(Super::PreInit());
		JPT_ENSURE(m_hInstance != nullptr);

		return true;
	}

	bool Application_Win64::Init()
	{
		JPT_ENSURE(Super::Init());

		if (CommandLine::GetInstance().Has("no_window"))
		{
			m_shouldTerminate = true;
			return true;
		}

		return true;
	}
}

#endif // IS_PLATFORM_WIN64