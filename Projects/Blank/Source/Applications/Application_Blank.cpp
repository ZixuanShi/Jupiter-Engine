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
const char* jpt::File::GetClientDir() { return JPT_CLIENT_DIR; }
const wchar_t* jpt::File::GetClientDirW() { return JPT_CLIENT_DIR_W; }
#pragma endregion