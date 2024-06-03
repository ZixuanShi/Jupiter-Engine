﻿// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTestsApplication;

import jpt.CoreModules;
import jpt.Application_Base;
import jpt.Utilities;

import UnitTests_Core;
import UnitTests_Debugging;
import UnitTests_System;

import jpt.String;

export class Application_JupiterUnitTests final : public jpt::Application_Base
{
private:
	using Super = jpt::Application_Base;

public:
	virtual bool Init() override final;
};

bool Application_JupiterUnitTests::Init()
{
	JPT_ENSURE(Super::Init());

	JPT_LOG("Core      Unit Tests %s", RunUnitTests_Core()      ? "Succeeded" : "Failed");
	JPT_LOG("Debugging Unit Tests %s", RunUnitTests_Debugging() ? "Succeeded" : "Failed");
	JPT_LOG("System    Unit Tests %s", RunUnitTests_System()    ? "Succeeded" : "Failed");

	return true;
}