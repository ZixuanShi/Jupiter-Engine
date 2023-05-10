#pragma once
#include "Application.h"

namespace jpt
{
	extern Application CreateApplication();
}

int main()
{
	jpt::Application pApplication = jpt::CreateApplication();
	if (pApplication.Init())
	{
		pApplication.Update();
	}
	pApplication.Clean();

	return 0;
}
