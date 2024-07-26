// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_WIN64
	#include <windows.h>
#endif

export module jpt.Environment;

export namespace jpt
{
	bool IsDebuggerPresent()
	{
#if IS_PLATFORM_WIN64
		return ::IsDebuggerPresent();
#else
		return false;
#endif
	}
}