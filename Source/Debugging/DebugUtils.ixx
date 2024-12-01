// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#if IS_PLATFORM_WIN64
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

export module jpt.Debug.Utils;

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