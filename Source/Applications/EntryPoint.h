#pragma once
#include "Application.h"

extern jpt::Application CreateApplication();

int main()
{
	jpt::Application application = CreateApplication();
	if (application.Init())
	{
		application.Update();
	}
	application.Clean();

	return 0;
}
