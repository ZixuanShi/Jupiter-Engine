// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

#if IS_PLATFORM_WIN64
	#include "Applications/App/Application_Win64.h"
#else
	#include "Applications/App/Application.h"
#endif

import jpt.Time.TypeDefs;

import Entity.Foo;
import Entity.Bar;

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

	Entity_Foo m_foo;
	Entity_Bar m_bar;

public:
	virtual bool PreInit() override;
	virtual void Update(jpt::TimePrecision deltaSeconds) override;
};