// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Data;

import jpt.TypeDefs;
import jpt.Utilities;

/** Unit Test Modules */

// Json
import UnitTests_Json;


export bool RunUnitTests_Data()
{
	/** Unit Test Functions */

	// Json
	JPT_ENSURE(RunUnitTests_Json());
    

	return true;
}
