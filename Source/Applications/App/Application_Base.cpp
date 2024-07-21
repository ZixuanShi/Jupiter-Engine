// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/App/Application_Base.h"

namespace jpt
{
	void Application_Base::Run()
	{
		while (!m_shouldTerminate)
		{
			PollInput();
			Update();
			Render();
		}
	}
}