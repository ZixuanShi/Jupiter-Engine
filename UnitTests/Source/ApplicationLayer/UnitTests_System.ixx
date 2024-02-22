// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_System;

import jpt.Utilities;

/** Unit Test Modules */

// Time
import UnitTests_Clock;
import UnitTests_StopWatch;

export bool RunUnitTests_System()
{
	/** Unit Test Functions */

	// Time
	JPT_ENSURE(RunUnitTests_Clock());
	JPT_ENSURE(RunUnitTests_StopWatch());

	return true;
}
