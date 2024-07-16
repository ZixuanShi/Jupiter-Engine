// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Core/Minimal/CoreHeaders.h"
#include "Profiling/TimingProfiler.h"

#include "Application_JupiterUnitTests.h"

import jpt.EntryPoints;

/** Main entry point for different platforms */
#if IS_PLATFORM_WIN64

#include <Windows.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lPStr, int nCmdShow)
{
	JPT_SCOPED_TIMING_PROFILER("Main");

	JPT_LOG(L"你好，世界 %s!", L"Jupiter Engine引擎");

	return jpt::MainImplWin64(hInstance, hPrevInstance, lPStr, nCmdShow);
}
#else
	#error "Platform not supported"
#endif