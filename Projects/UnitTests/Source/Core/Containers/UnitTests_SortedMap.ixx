// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_SortedMap;

import jpt.Function;
import jpt.Pair;
import jpt.SortedMap;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_SortedMap()
{
    jpt::SortedMap<int32, int32> sortedMap;
    
    // Add
    sortedMap.Add(4, 4);
    sortedMap.Add(2, 2);
    sortedMap.Add(6, 6);
    sortedMap.Add(1, 1);
    sortedMap.Add(3, 3);
    sortedMap.Add(5, 5);
    sortedMap.Add(7, 7);

    // Erase
    //sortedMap.Erase(4);

    auto printer = [](int32 key, int32 value)
	{
        jpt::Pair<int32, int32> pair(key, value);
		JPT_LOG(pair);
	};

	sortedMap.PreOrderWalk(printer);

    JPT_LOG("--------------------------------------");

    sortedMap.InOrderWalk(printer);

    JPT_LOG("--------------------------------------");

    sortedMap.PostOrderWalk(printer);

    return true;
}

export bool RunUnitTests_SortedMap()
{
    JPT_ENSURE(UnitTest_SortedMap());

    return true;
}
