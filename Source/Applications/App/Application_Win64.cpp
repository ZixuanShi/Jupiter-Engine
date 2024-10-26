// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/App/Application_Win64.h"

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

namespace jpt
{
	bool Application_Win64::PreInit()
	{
		JPT_ENSURE(Super::PreInit());
		JPT_ENSURE(m_hInstance != nullptr);

		return true;
	}

	Application_Win64* GetWin64Application()
	{
		return static_cast<Application_Win64*>(GetApplication());
	}
}