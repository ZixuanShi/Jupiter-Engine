// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module jpt.UnitTestsApplication;

import jpt.CoreModules;
import jpt.ApplicationBase;

import CoreUnitTests;

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

	JPT_LOG("Core Unit Tests %s", RunUnitTests_Core() ? "succeeded" : "failed");

	return true;
}
