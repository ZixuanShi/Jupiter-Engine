﻿// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTestsApplication;

import jpt.ApplicationBase;
import jpt.Utilities;

import UnitTests_Core;
import UnitTests_Debugging;
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
	JPT_ENSURE(Super::Init());

	JPT_LOG("Core      Unit Tests %s", RunUnitTests_Core()      ? "Succeeded" : "Failed");
	JPT_LOG("Debugging Unit Tests %s", RunUnitTests_Debugging() ? "Succeeded" : "Failed");
	JPT_LOG("System    Unit Tests %s", RunUnitTests_System()    ? "Succeeded" : "Failed");

	return true;
}
