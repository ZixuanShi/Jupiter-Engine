// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/Application_Blank.h"
#include "Core/Minimal/CoreHeaders.h"

import jpt.CoreModules;

bool Application_Blank::PreInit()
{
	JPT_ENSURE(Super::PreInit());

	return true;
}

#pragma region Engine-Client Communications
const char* jpt::GetClientDir() { return JPT_CLIENT_DIR; }
const wchar_t* jpt::GetClientDirW() { return JPT_CLIENT_DIR_W; }

jpt::Application_Base* jpt::Application_Base::GetInstance()
{
	static Application_Blank s_instance;
	return &s_instance;
}
#pragma endregion