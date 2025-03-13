// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Data;

import jpt.TypeDefs;
import jpt.Utilities;

/** Unit Test Modules */


// CSV
import UnitTests_CSV;

// Json
import UnitTests_Json;


export bool RunUnitTests_Data()
{
    /** Unit Test Functions */


    // CSV
    JPT_ENSURE(RunUnitTests_CSV());

    // Json
    JPT_ENSURE(RunUnitTests_Json());
    

    return true;
}
