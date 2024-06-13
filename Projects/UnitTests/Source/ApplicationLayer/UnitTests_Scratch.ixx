// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Scratch;

/** Unit Test Modules */

// Coding
import UnitTests_Coding;

// MathPractice
import UnitTests_MathPractice;

export bool RunUnitTests_Scratch()
{
	/** Unit Test Functions */

	// Coding
	RunUnitTests_Coding();

	// MathPractice
	RunUnitTests_MathPractice();

	return true;
}
