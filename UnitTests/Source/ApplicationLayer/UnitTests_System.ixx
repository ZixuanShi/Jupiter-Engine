// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_System;

import jpt.Utilities;

/** Unit Test Modules */

// Timing
import UnitTests_Timer;
import UnitTests_Timing;

// Date Time
import UnitTests_DateTime;

export bool RunUnitTests_System()
{
	/** Unit Test Functions */

	// Timing
	//JPT_ENSURE(RunUnitTests_Timing());	// Will sleep the program to test timing
	//JPT_ENSURE(RunUnitTests_Timer());
    
	// Date Time
	JPT_ENSURE(RunUnitTests_DateTime());


	return true;
}
