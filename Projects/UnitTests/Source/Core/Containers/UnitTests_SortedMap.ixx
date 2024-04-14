// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_SortedMap;

import jpt.SortedMap;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_SortedMap()
{
    jpt::SortedMap<int32, int32> sortedMap;
    
    sortedMap.Add(1, 1);
    sortedMap.Add(0, 0);
    sortedMap.Add(2, 2);
    sortedMap.Add(4, 4);
    sortedMap.Add(3, 3);

    return true;
}

export bool RunUnitTests_SortedMap()
{
    JPT_ENSURE(UnitTest_SortedMap());

    return true;
}
