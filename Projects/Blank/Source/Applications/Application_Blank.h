#pragma once

#if IS_PLATFORM_WIN64
	#include "Applications/App/Application_Win64.h"
#else
	#include "Applications/App/Application_Base.h"
#endif

class Application_Blank final : 
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
	virtual bool PreInit() override final;
};
