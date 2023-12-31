#include "ApplicationLayer/Applications/EntryPoints.h"

#include "JupiterUnitTestsApplication.h"
#include "Core/Building/BuildingMacros.h"

import jpt.CoreMinimalModules;
import JupiterUnitTestsModules;

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
	jpt::Foo::PrintNumber();
	const int n = GetUnitTestsCount();
	JPT_IGNORE(n);
	return jpt::MainImplWin64(hInstance, hPrevInstance, lPStr, nCmdShow);
}

int main()
{
	return jpt::MainImplWin64(nullptr, nullptr, nullptr, 0);
}

#endif