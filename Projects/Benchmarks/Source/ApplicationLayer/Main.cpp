// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Core/Minimal/Headers.h"

import jpt.CoreModules;
import jpt.EntryPoints;

import BenchmarksApplication;

/** Main entry point for different platforms */
#if IS_PLATFORM_WIN64

#include <Windows.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lPStr, int nCmdShow)
{
	JPT_SCOPED_TIMING_PROFILER("Main");
	JPT_LOG("Benchmarking Results:");

	return jpt::MainImplWin64(hInstance, hPrevInstance, lPStr, nCmdShow);
}
#endif
