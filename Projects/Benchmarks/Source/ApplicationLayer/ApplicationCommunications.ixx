// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// This file overrides the global communication functions through out both engine and client

module;

#include "ApplicationLayer/Application_Benchmarks.h"

#if IS_PLATFORM_WIN64
#include <Windows.h>
#endif

export module ApplicationCommunications;

import jpt.File.Path.Helpers;
import jpt.EntryPoints;

/** Must Overrides GetClientDir here */
constexpr const wchar_t* jpt::File::GetOutputDirW()
{
	return JPT_OUTPUT_DIR_W;
}

constexpr const wchar_t* jpt::File::GetClientDirW()
{
	return JPT_CLIENT_DIR_W;
}

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
	Application_JupiterUnitTests app;
	return jpt::MainImpl(&app);
}
#endif