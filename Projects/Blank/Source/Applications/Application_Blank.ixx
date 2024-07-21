// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module Application_Blank;

#if IS_PLATFORM_WIN64
import jpt.Application_Win64;
#else
import jpt.Application_Base;
#endif

import jpt.Utilities;
import jpt.File.Path;

export class Application_Blank final : 
#if IS_PLATFORM_WIN64
	public jpt::Application_Win64
#else
	public jpt::Application_Base
#endif
{
private:
	#if IS_PLATFORM_WIN64
		using Super = jpt::Application_Win64;
	#else
		using Super = jpt::Application_Base;
	#endif

public:
	virtual bool PreInit() override;
};

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


