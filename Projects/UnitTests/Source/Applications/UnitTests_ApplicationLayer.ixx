// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_ApplicationLayer;

import jpt.TypeDefs;
import jpt.Utilities;

/** Unit Test Modules */

// Applications
import UnitTests_LaunchArgs;


export bool RunUnitTests_ApplicationLayer()
{
	/** Unit Test Functions */

	// Applications
	JPT_ENSURE(RunUnitTests_LaunchArgs());
    

	return true;
}
