// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module jpt.UnitTestsApplication;

import jpt.CoreModules;
import jpt.ApplicationBase;

import UnitTests_Core;
import UnitTests_System;

export class JupiterUnitTestsApplication final : public jpt::ApplicationBase
{
private:
	using Super = jpt::ApplicationBase;

public:
	virtual bool Init() override final;
};

bool JupiterUnitTestsApplication::Init()
{
	JPT_RETURN_FALSE_IF_ERROR(!Super::Init(), "Failed Super::Init()");

	JPT_LOG("Core Unit Tests %s",   RunUnitTests_Core()   ? "Succeeded" : "Failed");
	JPT_LOG("System Unit Tests %s", RunUnitTests_System() ? "Succeeded" : "Failed");

	return true;
}
