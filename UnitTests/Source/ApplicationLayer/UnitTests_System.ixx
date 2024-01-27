// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module UnitTests_System;

/** Unit Test Modules */

// Timing
import UnitTests_Clock;


export bool RunUnitTests_System()
{
	/** Unit Test Functions */

	// Timing
	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_Clock(), "Clock Unit Tests Failed");
    

	return true;
}
