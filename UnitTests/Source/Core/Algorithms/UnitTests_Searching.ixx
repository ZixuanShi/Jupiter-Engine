// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Searching;

import jpt.Searching;
import jpt.Utilities;
import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.StaticArray;
import jpt.LinkedList;
import jpt.String;

bool UnitTest_Searching_StaticArray()
{
    jpt::StaticArray<int32, 5> staticArray = { 1, 2, 3, 4, 5 };

    JPT_RETURN_FALSE_IF_ERROR(jpt::Find(staticArray, 6) != staticArray.end(), "");
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(staticArray, 5) , "");
    JPT_RETURN_FALSE_IF_ERROR(jpt::FindIf(staticArray, [](int32 n) {return n == 8; }) != staticArray.end(), "");

    jpt::StaticArray<jpt::String, 5> staticStrArray = { "Zero", "One", "Two", "Three", "Four" };

    JPT_RETURN_FALSE_IF_ERROR(jpt::Find(staticStrArray, "Six") != staticStrArray.end(), "");
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(staticStrArray, "Zero"), "");
    JPT_RETURN_FALSE_IF_ERROR(jpt::FindIf(staticStrArray, [](const jpt::String& str) {return str == "Eight"; }) != staticStrArray.end(), "");

    return true;
}

bool UnitTest_Searching_DynamicArray()
{
    jpt::DynamicArray<int32> dynamicArray = { 1, 2, 3, 4, 5 };

    JPT_RETURN_FALSE_IF_ERROR(jpt::Find(dynamicArray, 6) != dynamicArray.end(), "");
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(dynamicArray, 5), "");

    JPT_RETURN_FALSE_IF_ERROR(jpt::Contains(dynamicArray, 7), "");
    dynamicArray.EmplaceBack(7);
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(dynamicArray, 7), "");

    *jpt::Find(dynamicArray, 7) = 8;
    JPT_RETURN_FALSE_IF_ERROR(jpt::Contains(dynamicArray, 7), "");
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(dynamicArray, 8), "");

    JPT_RETURN_FALSE_IF_ERROR(jpt::FindIf(dynamicArray, [](int32 n) {return n == 8; }) == dynamicArray.end(), "");

    jpt::DynamicArray<jpt::String> dynamicStrArray = { "Zero", "One", "Two", "Three", "Four" };

    JPT_RETURN_FALSE_IF_ERROR(jpt::Find(dynamicStrArray, "Six") != dynamicStrArray.end(), "");
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(dynamicStrArray, "Zero"), "");

    JPT_RETURN_FALSE_IF_ERROR(jpt::Contains(dynamicStrArray, "Seven"), "");
    dynamicStrArray.EmplaceBack("Seven");
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(dynamicStrArray, "Seven"), "");

    *jpt::Find(dynamicStrArray, "Seven") = "Eight";
    JPT_RETURN_FALSE_IF_ERROR(jpt::Contains(dynamicStrArray, "Seven"), "");
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(dynamicStrArray, "Eight"), "");

    JPT_RETURN_FALSE_IF_ERROR(jpt::FindIf(dynamicStrArray, [](const jpt::String& str) {return str == "Eight"; }) == dynamicStrArray.end(), "");

    return true;
}

bool UnitTest_Searching_LinkedList()
{
    jpt::LinkedList<int32> linkedList = { 1, 2, 3, 4, 5 };

    JPT_RETURN_FALSE_IF_ERROR(jpt::Find(linkedList, 6) != linkedList.end(), "");
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(linkedList, 5), "");

    JPT_RETURN_FALSE_IF_ERROR(jpt::Contains(linkedList, 7), "");
    linkedList.EmplaceBack(7);
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(linkedList, 7), "");

    *jpt::Find(linkedList, 7) = 8;
    JPT_RETURN_FALSE_IF_ERROR(jpt::Contains(linkedList, 7), "");
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(linkedList, 8), "");

    JPT_RETURN_FALSE_IF_ERROR(jpt::FindIf(linkedList, [](int32 n) {return n == 8; }) == linkedList.end(), "");

    jpt::DynamicArray<jpt::String> linkedListStr = { "Zero", "One", "Two", "Three", "Four" };

    JPT_RETURN_FALSE_IF_ERROR(jpt::Find(linkedListStr, "Six") != linkedListStr.end(), "");
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(linkedListStr, "Zero"), "");

    JPT_RETURN_FALSE_IF_ERROR(jpt::Contains(linkedListStr, "Seven"), "");
    linkedListStr.EmplaceBack("Seven");
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(linkedListStr, "Seven"), "");

    *jpt::Find(linkedListStr, "Seven") = "Eight";
    JPT_RETURN_FALSE_IF_ERROR(jpt::Contains(linkedListStr, "Seven"), "");
    JPT_RETURN_FALSE_IF_ERROR(!jpt::Contains(linkedListStr, "Eight"), "");

    JPT_RETURN_FALSE_IF_ERROR(jpt::FindIf(linkedListStr, [](const jpt::String& str) {return str == "Eight"; }) == linkedListStr.end(), "");

    return true;
}

export bool RunUnitTests_Searching()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Searching_StaticArray(), "UnitTest_Searching_StaticArray Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Searching_DynamicArray(), "UnitTest_Searching_DynamicArray Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Searching_LinkedList(), "UnitTest_Searching_LinkedList Failed");

    return true;
}
