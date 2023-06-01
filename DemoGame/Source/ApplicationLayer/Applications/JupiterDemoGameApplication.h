#pragma once

#if IS_PLATFORM_WIN64
#include "ApplicationLayer/Applications/Platform_Win64/Win64Application.h"
#else
#error "Current platform's application is not implemented"
#endif

class JupiterDemoGameApplication final 
#if IS_PLATFORM_WIN64
	: public jpt::Win64Application
#else
	: public jpt::ApplicationBase
#endif
{ 
private:
#if IS_PLATFORM_WIN64
	using Super = jpt::Win64Application;
#else
	using Super = jpt::ApplicationBase;
#endif

public:
	virtual bool PreInit() override;
	virtual bool Init() override;
};

