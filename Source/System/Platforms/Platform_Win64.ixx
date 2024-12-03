// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module jpt.Platform.Win64;

#if IS_PLATFORM_WIN64

import jpt.Platform;
import jpt.TypeDefs;
import jpt.Utilities;

using HINSTANCE = struct HINSTANCE__*;

export namespace jpt
{
	class Platform_Win64 : public Platform
	{
	private:
		using Super = Platform;

	private:
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

	bool Platform_Win64::PreInit()
	{
		JPT_ENSURE(Super::PreInit());
		JPT_ENSURE(m_hInstance != nullptr);

		return true;
	}
}

#endif // IS_PLATFORM_WIN64
