
// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

#include "ApplicationLayer/Application_JupiterUnitTests.h"

export module UnitTests_Coding_Scratch;

import jpt.CoreModules;

export bool UnitTest_Coding_Scratch()
{
	Application_JupiterUnitTests* app = static_cast<Application_JupiterUnitTests*>(Application_JupiterUnitTests::GetInstance());

	JPT_LOG(app->GetName());
	JPT_LOG(app->m_settings.Width);

	return true;
}