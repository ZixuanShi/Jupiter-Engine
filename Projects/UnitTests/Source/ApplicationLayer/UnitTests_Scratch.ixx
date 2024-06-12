// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Scratch;

/** Unit Test Modules */

// MathPractice
import UnitTests_MathPractice;

export bool RunUnitTests_Scratch()
{
	/** Unit Test Functions */

	// MathPractice
	RunUnitTests_MathPractice();

	return true;
}
