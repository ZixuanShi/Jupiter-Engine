// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Applications;

import jpt.TypeDefs;
import jpt.Utilities;

/** Unit Test Modules */

// Applications
import UnitTests_CommandLine;


export bool RunUnitTests_Applications()
{
	/** Unit Test Functions */

	// Applications
	JPT_ENSURE(RunUnitTests_CommandLine());
    

	return true;
}
