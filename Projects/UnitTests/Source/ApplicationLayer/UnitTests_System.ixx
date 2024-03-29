// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_System;

import jpt.Utilities;

/** Unit Test Modules */

// FileIO
import UnitTests_FileIO;

// Time
import UnitTests_Clock;
import UnitTests_StopWatch;

export bool RunUnitTests_System()
{
	/** Unit Test Functions */

	// FileIO
	JPT_ENSURE(RunUnitTests_FileIO());

	// Time
	JPT_ENSURE(RunUnitTests_Clock());
	JPT_ENSURE(RunUnitTests_StopWatch());

	return true;
}
