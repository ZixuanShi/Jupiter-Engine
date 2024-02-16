// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_System;

/** Unit Test Modules */

// Timing
import UnitTests_Timing;


export bool RunUnitTests_System()
{
	/** Unit Test Functions */

	// Timing
	//JPT_ENSURE(RunUnitTests_Timing(), "Timing Unit Tests Failed");	// Will sleep the program to test timing
    

	return true;
}
