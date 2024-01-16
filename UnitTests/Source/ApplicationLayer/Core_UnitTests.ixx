// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module CoreUnitTests;

// Utilities
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
import EnumUnitTests;

export bool RunUnitTests_Core()
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
	JPT_RETURN_FALSE_IF_ERROR(!RunEnumUnitTests(), "Enum Tests Failed");

	return true;
}