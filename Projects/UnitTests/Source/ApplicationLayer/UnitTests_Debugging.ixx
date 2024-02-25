// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Debugging;

/** Unit Test Modules */

// Logging
import UnitTests_Logger;
import jpt.Utilities;

export bool RunUnitTests_Debugging()
{
	/** Unit Test Functions */

	// Timing
	JPT_ENSURE(RunUnitTests_Logger());
    

	return true;
}
