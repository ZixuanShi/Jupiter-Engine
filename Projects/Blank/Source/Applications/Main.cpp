// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/Application_Blank.h"

import jpt.CommandLine;
import jpt.EntryPoints;

#if IS_PLATFORM_WIN64

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR launchArgs, int nCmdShow)
{
	jpt::CommandLine::GetInstance().Parse(launchArgs);

	Application_Blank app;
	app.SetHINSTANCE(hInstance);
	app.SetnCmdShow(nCmdShow);

	return jpt::MainImpl(&app);
}
#else

int main(int argc, char* argv[])
{
	jpt::CommandLine::GetInstance().Parse(argc, argv);

	Application_Blank app;
	return jpt::MainImpl(&app);
}
#endif