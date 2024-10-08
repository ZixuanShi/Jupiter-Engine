// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/Application_Benchmarks.h"

import jpt.EntryPoints;

#if IS_PLATFORM_WIN64

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR launchArgs, int nCmdShow)
{
	Application_Benchmarks app;
	return jpt::MainImpl_Win64(&app, hInstance, launchArgs, nCmdShow);
}
#else

int main(int argc, char* argv[])
{
	Application_Benchmarks app;
	return jpt::MainImpl(&app);
}
#endif