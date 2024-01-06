// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

export module jpt.UnitTestsApplication;

import jpt.CoreModules;
import jpt.ApplicationBase;

// Math
import MathUnitTests;
import RandomNumberGeneratorUnitTests;

// Memory Managing
import AllocatorUnitTests;

// Strings
import StringUtilsUnitTests;
import StringUnitTests;

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
	RunMathUnitTests();
	RunRandomNumberGeneratorUnitTests();

	// Memory Managing
	RunAllocatorUnitTests();

	// Strings
	RunStringUtilsUnitTests();
	RunStringUnitTests();
}
