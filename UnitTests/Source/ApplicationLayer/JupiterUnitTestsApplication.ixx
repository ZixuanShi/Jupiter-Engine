// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

export module jpt.UnitTestsApplication;

import jpt.CoreModules;
import jpt.ApplicationBase;

import AllocatorUnitTests;
import RandomNumberGeneratorUnitTests;

export class JupiterUnitTestsApplication final : public jpt::ApplicationBase
{
private:
	using Super = jpt::ApplicationBase;

public:
	virtual bool Init() override final;

private:
	virtual void RunUnitTests_Core() const;
};

bool JupiterUnitTestsApplication::Init()
{
	JPT_RETURN_FALSE_IF_LOG(!Super::Init(), "Failed Super::Init()");

	RunUnitTests_Core();

	return true;
}

void JupiterUnitTestsApplication::RunUnitTests_Core() const
{
	// Math
	RunRandomNumberGeneratorUnitTests();

	// Memory Managing
	RunAllocatorUnitTests();
}
