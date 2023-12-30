#include "ApplicationLayer/Applications/EntryPoints.h"

#include "JupiterUnitTestsApplication.h"

/** Must Overrides Application GetInstance here */
jpt::ApplicationBase& jpt::ApplicationBase::GetInstance()
{
	static JupiterUnitTestsApplication s_instance;
	return s_instance;
}

/** Main entry point for different platforms */
#if IS_PLATFORM_WIN64

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lPStr, int nCmdShow)
{
	return jpt::MainImpl(hInstance, hPrevInstance, lPStr, nCmdShow);
}

#endif