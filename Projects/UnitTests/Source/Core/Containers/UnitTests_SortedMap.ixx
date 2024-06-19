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
    jpt::SortedMap<int32, char> sortedMap;
    
    // Add
    sortedMap.Add(1, 'E');
    sortedMap.Add(0, 'A');
    sortedMap.Add(2, 'D');
    sortedMap.Add(4, 'G');
    sortedMap.Add(3, 'B');
    sortedMap.Add(6, 'E');

    // Erase
    //sortedMap.Erase(4);

    auto printer = [](int32 key, char value)
	{
        jpt::Pair<int32, char> pair(key, value);
		JPT_LOG(pair);
	};

	sortedMap.TraversePreOrder(printer);

    JPT_LOG("--------------------------------------");

    sortedMap.TraverseInOrder(printer);

    JPT_LOG("--------------------------------------");

    sortedMap.TraversePostOrder(printer);

    return true;
}

export bool RunUnitTests_SortedMap()
{
    JPT_ENSURE(UnitTest_SortedMap());

    return true;
}
