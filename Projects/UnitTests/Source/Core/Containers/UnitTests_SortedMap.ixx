// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_SortedMap;

import jpt.SortedMap;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_SortedMap()
{

    return true;
}

export bool RunUnitTests_SortedMap()
{
    JPT_ENSURE(UnitTest_SortedMap());

    return true;
}
