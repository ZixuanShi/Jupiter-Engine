// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_SortedMap;

import jpt.Function;
import jpt.Pair;
import jpt.SortedMap;
import jpt.HashMap;
import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_SortedMap_Add()
{
    jpt::SortedMap<int32, char> sortedMap;
    sortedMap.Add(4, '4');
    sortedMap.Add(2, '2');
    sortedMap.Add(6, '6');
    sortedMap.Add(1, '1');
    sortedMap.Add(3, '3');
    sortedMap.Add(5, '5');
    sortedMap.Add(7, '7');

    jpt::DynamicArray<jpt::Pair<int32, char>> dynamicArray;
    dynamicArray.EmplaceBack(1, '1');
    dynamicArray.EmplaceBack(2, '2');
    dynamicArray.EmplaceBack(3, '3');
    dynamicArray.EmplaceBack(4, '4');
    dynamicArray.EmplaceBack(5, '5');
    dynamicArray.EmplaceBack(6, '6');
    dynamicArray.EmplaceBack(7, '7');

    JPT_ENSURE(sortedMap.Count() == dynamicArray.Count());

    size_t i = 0;
    sortedMap.InOrderWalk([&dynamicArray, &i](int32 key, char value)
	{
        JPT_ENSURE((dynamicArray[i] == jpt::Pair<int32, char>(key, value)));
		++i;
        return true;
	});

    return true;
}

bool UnitTest_SortedMap_Walk()
{
	jpt::SortedMap<int32, char> sortedMap;
	sortedMap.Add(4, '4');
	sortedMap.Add(2, '2');
	sortedMap.Add(6, '6');
	sortedMap.Add(1, '1');
	sortedMap.Add(3, '3');
	sortedMap.Add(5, '5');
	sortedMap.Add(7, '7');

    auto printer = [](int32 key, char value)
	{
        jpt::Pair<int32, char> pair(key, value);
		JPT_LOG(pair);
	};

	sortedMap.PreOrderWalk(printer);

    JPT_LOG("--------------------------------------");

    sortedMap.InOrderWalk(printer);

    JPT_LOG("--------------------------------------");

    sortedMap.PostOrderWalk(printer);

	return true;
}

bool UnitTest_SortedMap_Iterators()
{
    jpt::SortedMap<int32, char> sortedMap;
    sortedMap.Add(4, '4');
    sortedMap.Add(2, '2');
    sortedMap.Add(6, '6');
    sortedMap.Add(1, '1');
    sortedMap.Add(3, '3');
    sortedMap.Add(5, '5');
    sortedMap.Add(7, '7');

    for (const auto& [key, value] : sortedMap)
	{
		jpt::Pair<int32, char> pair(key, value);
        JPT_LOG(pair);
	}

    return true;
}

export bool RunUnitTests_SortedMap()
{
    JPT_ENSURE(UnitTest_SortedMap_Add());
    //JPT_ENSURE(UnitTest_SortedMap_Walk());
    JPT_ENSURE(UnitTest_SortedMap_Iterators());

    return true;
}
