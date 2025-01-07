// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_SortedMap;

import jpt.Function;
import jpt.Pair;
import jpt.SortedMap;
import jpt.HashMap;
import jpt.DynamicArray;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.String;

bool UnitTests_SortedMap_Copy()
{
    jpt::SortedMap<int32, jpt::DynamicArray<jpt::String>> sortedMap;
    sortedMap.Add(4, { "Unity", "Unreal", "CryEngine" });
    sortedMap.Add(1, { "C++",  "Python", "Lua" });
    sortedMap.Add(3, { "DX12", "DX11",   "Vulkan", "OpenGL" });
    sortedMap.Add(2, { "C#",   "Java",   "JavaScript" });

    jpt::SortedMap<int32, jpt::DynamicArray<jpt::String>> sortedMapCopy(sortedMap);

    for (const auto& [key, value] : sortedMap)
	{
		JPT_ENSURE(sortedMapCopy.Has(key));
		JPT_ENSURE(sortedMapCopy[key] == value);
	}
    
    return true;
}

bool UnitTests_SortedMap_Move()
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

bool UnitTests_SortedMap_Add()
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

bool UnitTests_SortedMap_Erase()
{
    jpt::SortedMap<int32, char> sortedMap;
    sortedMap[4] = '4';
    sortedMap[2] = '2';
    sortedMap[6] = '6';
    sortedMap[1] = '1';
    sortedMap[3] = '3';
    sortedMap[5] = '5';
    sortedMap[7] = '7';
	sortedMap[8] = '8';
	sortedMap[9] = '9';

    sortedMap.Erase(1);
    sortedMap.Erase(2);
    sortedMap.Erase(3);
    sortedMap.Erase(4);
    sortedMap.Erase(5);
    sortedMap.Erase(6);
    sortedMap.Erase(7);
    sortedMap.Erase(8);
    sortedMap.Erase(9);

    JPT_ENSURE(!sortedMap.Has(1));
    JPT_ENSURE(!sortedMap.Has(2));
    JPT_ENSURE(!sortedMap.Has(3));
    JPT_ENSURE(!sortedMap.Has(4));
    JPT_ENSURE(!sortedMap.Has(5));
    JPT_ENSURE(!sortedMap.Has(6));
    JPT_ENSURE(!sortedMap.Has(7));
    JPT_ENSURE(!sortedMap.Has(8));
    JPT_ENSURE(!sortedMap.Has(9));

    return true;
}

bool UnitTests_SortedMap_Walk()
{
	jpt::SortedMap<int32, char> sortedMap;
    /** Traverse
               A
            /    \
          /        \
         B          C
       /  \       /  \
      D    E     F    G
     / \  / \   / \  / \
    H  I  J  K  L  M N  O 			*/

    sortedMap.Add(8, 'A');
    sortedMap.Add(4, 'B');
    sortedMap.Add(12, 'C');
    sortedMap.Add(2, 'D');
    sortedMap.Add(6, 'E');
    sortedMap.Add(10, 'F');
    sortedMap.Add(14, 'G');
    sortedMap.Add(1, 'H');
    sortedMap.Add(3, 'I');
    sortedMap.Add(5, 'J');
    sortedMap.Add(7, 'K');
    sortedMap.Add(9, 'L');
    sortedMap.Add(11, 'M');
    sortedMap.Add(13, 'N');
    sortedMap.Add(15, 'O');

    JPT_LOG("-------------------------------------------------\nPre Order");
    sortedMap.PreOrderWalk([](int32 key, char value)
        {
			JPT_LOG("Key: %i, Value: %c", key, value);
        });
    JPT_LOG("-------------------------------------------------\nIn Order");
	sortedMap.InOrderWalk([](int32 key, char value)
		{
			JPT_LOG("Key: %i, Value: %c", key, value);
		});
    JPT_LOG("-------------------------------------------------\nPost Order");
    sortedMap.PostOrderWalk([](int32 key, char value)
		{
			JPT_LOG("Key: %i, Value: %c", key, value);
		});
	JPT_LOG("-------------------------------------------------\nBFS");
	sortedMap.BFS([](int32 key, char value)
		{
			JPT_LOG("Key: %i, Value: %c", key, value);
		});

	return true;
}

bool UnitTests_SortedMap_Iterators()
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

bool UnitTests_SortedMap_Unique()
{
	jpt::SortedMap<int32, jpt::String> sortedMap;
    sortedMap[1] = "one";
	sortedMap[2] = "two";
	sortedMap[3] = "three";

	JPT_ENSURE(sortedMap.Count() == 3);
	JPT_ENSURE(sortedMap[1] == "one");
	JPT_ENSURE(sortedMap[2] == "two");
	JPT_ENSURE(sortedMap[3] == "three");

	sortedMap.Add(1, "four");
	sortedMap[2] = "five";

	JPT_ENSURE(sortedMap.Count() == 3);
	JPT_ENSURE(sortedMap[1] == "four");
	JPT_ENSURE(sortedMap[2] == "five");
	JPT_ENSURE(sortedMap[3] == "three");

	return true;
}

export bool RunUnitTests_SortedMap()
{
    JPT_ENSURE(UnitTests_SortedMap_Copy());
    JPT_ENSURE(UnitTests_SortedMap_Move());

    JPT_ENSURE(UnitTests_SortedMap_Add());
    JPT_ENSURE(UnitTests_SortedMap_Erase());
    //JPT_ENSURE(UnitTests_SortedMap_Walk());
    JPT_ENSURE(UnitTests_SortedMap_Iterators());
    JPT_ENSURE(UnitTests_SortedMap_Unique());

    return true;
}
