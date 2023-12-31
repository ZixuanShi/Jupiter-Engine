// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "ApplicationBase.h"

import jpt.CoreMinimalModules;

namespace jpt
{
	bool ApplicationBase::Init()
	{
		return true;
	}
	
	void ApplicationBase::Update()
	{
	}
	
	void ApplicationBase::Clean()
	{
		Foo::PrintNumber();
	}
}
