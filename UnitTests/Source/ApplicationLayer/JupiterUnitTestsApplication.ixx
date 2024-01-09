// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

export module jpt.UnitTestsApplication;

import jpt.CoreModules;
import jpt.ApplicationBase;

// Building
import UtilitiesUnitTests;

// Math
import MathUnitTests;
import RandomNumberGeneratorUnitTests;

// Memory Managing
import AllocatorUnitTests;
import UniquePtrUnitTests;

// Strings
import StringUtilsUnitTests;
import StringUnitTests;

// Timing
import TimingUtilsUnitTests;

export class JupiterUnitTestsApplication final : public jpt::ApplicationBase
{
private:
	using Super = jpt::ApplicationBase;

public:
	virtual bool Init() override final;

private:
	bool RunUnitTests_Core() const;
};

bool JupiterUnitTestsApplication::Init()
{
	JPT_RETURN_FALSE_IF_ERROR(!Super::Init(), "Failed Super::Init()");

	JPT_LOG_IF(RunUnitTests_Core(), "Core Unit Tests succeeded");

	return true;
}

bool JupiterUnitTestsApplication::RunUnitTests_Core() const
{
	// Building
	JPT_RETURN_FALSE_IF_ERROR(!RunUtilitiesUnitTests(), "Utilities Unit Tests Failed");

	// Math
	JPT_RETURN_FALSE_IF_ERROR(!RunMathUnitTests(), "Math Unit Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunRandomNumberGeneratorUnitTests(), "Random Number Generator Tests Failed");

	// Memory Managing
	JPT_RETURN_FALSE_IF_ERROR(!RunAllocatorUnitTests(), "Allocator Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUniquePtrUnitTests(), "UniquePtr Tests Failed");

	// Strings
	JPT_RETURN_FALSE_IF_ERROR(!RunStringUtilsUnitTests(), "StringUtils Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunStringUnitTests(), "String Tests Failed");

	// Timing
	JPT_RETURN_FALSE_IF_ERROR(!RunTimingUtilsUnitTests(), "TimingUtils Tests Failed");

	return true;
}
