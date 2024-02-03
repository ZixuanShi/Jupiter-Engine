// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_HashMap;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.HashMap;

bool UnitTest_HashMap_Trivial()
{

    return true;
}

export bool RunUnitTests_HashMap()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_HashMap_Trivial(), "UnitTest_HashMap_Trivial Failed");

    return true;
}
