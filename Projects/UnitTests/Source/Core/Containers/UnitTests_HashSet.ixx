// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_HashSet;

import jpt.HashSet;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_HashSet()
{

    return true;
}

export bool RunUnitTests_HashSet()
{
    JPT_ENSURE(UnitTest_HashSet());

    return true;
}
