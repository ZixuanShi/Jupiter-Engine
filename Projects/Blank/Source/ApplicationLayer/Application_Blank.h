#pragma once

#if IS_PLATFORM_WIN64
	import jpt.Application_Win64;
#else
	import jpt.Application_Base;
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
	virtual bool Init() override final;
};
