// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module jpt.UnitTestsApplication;

import jpt.CoreModules;
import jpt.ApplicationBase;

// Building
import UtilitiesUnitTests;

// Debugging
import LoggerUnitTests;

// Math
import MathUnitTests;
import RandomNumberGeneratorUnitTests;

// Memory Managing
import AllocatorUnitTests;
import StrongPtrUnitTests;
import UniquePtrUnitTests;
import WeakPtrUnitTests;

// Strings
import StringUtilsUnitTests;
import StringUnitTests;
import StringViewUnitTests;

// Timing
import TimingUtilsUnitTests;

// Types
import OptionalUnitTests;

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

	// Debugging
	JPT_RETURN_FALSE_IF_ERROR(!RunLoggerUnitTests(), "Logger Unit Tests Failed");

	// Math
	JPT_RETURN_FALSE_IF_ERROR(!RunMathUnitTests(), "Math Unit Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunRandomNumberGeneratorUnitTests(), "Random Number Generator Tests Failed");

	// Memory Managing
	JPT_RETURN_FALSE_IF_ERROR(!RunAllocatorUnitTests(), "Allocator Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunStrongPtrUnitTests(), "StrongPtr Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunUniquePtrUnitTests(), "UniquePtr Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunWeakPtrUnitTests(), "WeakPtr Tests Failed");

	// Strings
	JPT_RETURN_FALSE_IF_ERROR(!RunStringUnitTests(), "String Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunStringUtilsUnitTests(), "StringUtils Tests Failed");
	JPT_RETURN_FALSE_IF_ERROR(!RunStringViewUnitTests(), "StringView Tests Failed");

	// Timing
	JPT_RETURN_FALSE_IF_ERROR(!RunTimingUtilsUnitTests(), "TimingUtils Tests Failed");

	// Types
	JPT_RETURN_FALSE_IF_ERROR(!RunOptionalUnitTests(), "Optional Tests Failed");

	return true;
}
