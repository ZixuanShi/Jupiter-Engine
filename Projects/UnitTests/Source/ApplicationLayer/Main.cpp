﻿// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Core/Minimal/Headers.h"

import jpt.CoreModules;
import jpt.EntryPoints;
import jpt.UnitTestsApplication;

/** Must Overrides Application GetInstance here */
jpt::ApplicationBase& jpt::ApplicationBase::GetInstance()
{
	static JupiterUnitTestsApplication s_instance;
	return s_instance;
}

/** Main entry point for different platforms */
#if IS_PLATFORM_WIN64

#include <Windows.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lPStr, int nCmdShow)
{
	JPT_LOG(L"你好，世界 %s!", L"Jupiter Engine引擎");

	return jpt::MainImplWin64(hInstance, hPrevInstance, lPStr, nCmdShow);
}
#endif