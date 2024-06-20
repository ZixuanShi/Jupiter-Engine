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
import jpt.String;

bool UnitTest_SortedMap_Copy()
{
    jpt::SortedMap<int32, jpt::DynamicArray<jpt::String>> sortedMap;
    sortedMap.Add(4, { "Unity", "Unreal", "CryEngine" });
    sortedMap.Add(1, { "C++",  "Python", "Lua" });
    sortedMap.Add(3, { "DX12", "DX11",   "Vulkan", "OpenGL" });
    sortedMap.Add(2, { "C#",   "Java",   "JavaScript" });

    jpt::SortedMap<int32, jpt::DynamicArray<jpt::String>> sortedMapCopy(sortedMap);

    for (const auto& [key, value] : sortedMap)
	{
		JPT_ENSURE(sortedMapCopy.Contains(key));
		JPT_ENSURE(sortedMapCopy[key] == value);
	}
    
    return true;
}

bool UnitTest_SortedMap_Move()
{
    jpt::SortedMap<int32, jpt::DynamicArray<jpt::String>> sortedMap;
    sortedMap.Add(4, { "Unity", "Unreal", "CryEngine" });
    sortedMap.Add(1, { "C++",  "Python", "Lua" });
    sortedMap.Add(3, { "DX12", "DX11",   "Vulkan", "OpenGL" });
    sortedMap.Add(2, { "C#",   "Java",   "JavaScript" });

    jpt::SortedMap<int32, jpt::DynamicArray<jpt::String>> sortedMapMove(jpt::Move(sortedMap));

	JPT_ENSURE(sortedMap.Count() == 0);

    JPT_ENSURE(sortedMapMove.Count() == 4);
    JPT_ENSURE((sortedMapMove[1] == (jpt::DynamicArray<jpt::String>{"C++", "Python", "Lua"})));
    JPT_ENSURE((sortedMapMove[2] == (jpt::DynamicArray<jpt::String>{"C#", "Java", "JavaScript"})));
    JPT_ENSURE((sortedMapMove[3] == (jpt::DynamicArray<jpt::String>{"DX12", "DX11", "Vulkan", "OpenGL"})));
    JPT_ENSURE((sortedMapMove[4] == (jpt::DynamicArray<jpt::String>{"Unity", "Unreal", "CryEngine"})));

    return true;
}

bool UnitTest_SortedMap_Add()
{
    jpt::SortedMap<int32, char> sortedMap;
    sortedMap[4] = '4';
    sortedMap[2] = '2';
    sortedMap[6] = '6';
    sortedMap[1] = '1';
    sortedMap[3] = '3';
    sortedMap[5] = '5';
    sortedMap[7] = '7';

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

    int32 i = 1;
    for (const auto& [key, value] : sortedMap)
	{
        JPT_ENSURE(value == (key + '0'));
        JPT_ENSURE(key == i);
        ++i;
	}

    return true;
}

export bool RunUnitTests_SortedMap()
{
    JPT_ENSURE(UnitTest_SortedMap_Copy());
    JPT_ENSURE(UnitTest_SortedMap_Move());

    JPT_ENSURE(UnitTest_SortedMap_Add());
    //JPT_ENSURE(UnitTest_SortedMap_Walk());
    JPT_ENSURE(UnitTest_SortedMap_Iterators());

    return true;
}
