// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#if IS_PLATFORM_WIN64
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

module jpt.Debugger;

namespace jpt
{
	bool Debugger::PreInit()
	{
		bool success = true;

		success &= m_assertImpl.PreInit();

		return success;
	}

	bool Debugger::IsDebuggerPresent() const
	{
#if IS_PLATFORM_WIN64
		return ::IsDebuggerPresent();
#else
		return false;
#endif
	}
}