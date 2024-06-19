// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_SortedMap;

import jpt.SortedMap;
import jpt.TypeDefs;
import jpt.Pair;
import jpt.Utilities;

bool UnitTest_SortedMap()
{
    jpt::SortedMap<int32, char> sortedMap;
    
    sortedMap.Add(1, 'E');
    sortedMap.Add(0, 'A');
    sortedMap.Add(2, 'D');
    sortedMap.Add(4, 'G');
    sortedMap.Add(3, 'B');
    sortedMap.Add(6, 'E');

	sortedMap.TraversePreOrder([](jpt::Pair<int32, char>& pair)
		{
			JPT_LOG(pair);
		});
    sortedMap.TraverseInOrder([](jpt::Pair<int32, char>& pair)
        {
            JPT_LOG(pair);
        });
    sortedMap.TraversePostOrder([](jpt::Pair<int32, char>& pair)
        {
            JPT_LOG(pair);
        });

    return true;
}

export bool RunUnitTests_SortedMap()
{
    JPT_ENSURE(UnitTest_SortedMap());

    return true;
}
