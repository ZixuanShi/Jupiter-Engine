// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Searching;

import jpt.LinearFind;
import jpt.Utilities;
import jpt.TypeDefs;
import jpt.DynamicArray;
import jpt.StaticArray;
import jpt.LinkedList;
import jpt.String;

bool UnitTests_Searching_StaticArray()
{
    const jpt::StaticArray<int32, 5> staticArray = { 1, 2, 3, 4, 5 };

    JPT_ENSURE(jpt::Find(staticArray, 6) == staticArray.end());
    JPT_ENSURE(jpt::Has(staticArray, 5) );
    JPT_ENSURE(jpt::FindIf(staticArray, [](int32 n) {return n == 8; }) == staticArray.end());

    jpt::StaticArray<jpt::String, 5> staticStrArray = { "Zero", "One", "Two", "Three", "Four" };

    JPT_ENSURE(jpt::Find(staticStrArray, "Six") == staticStrArray.end());
    JPT_ENSURE(jpt::Has(staticStrArray, "Zero"));
    JPT_ENSURE(jpt::FindIf(staticStrArray, [](const jpt::String& str) {return str == "Eight"; }) == staticStrArray.end());

    return true;
}

bool UnitTests_Searching_DynamicArray()
{
    jpt::DynamicArray<int32> dynamicArray = { 1, 2, 3, 4, 5 };

    JPT_ENSURE(jpt::Find(dynamicArray, 6) == dynamicArray.end());
    JPT_ENSURE(jpt::Has(dynamicArray, 5));

    JPT_ENSURE(!jpt::Has(dynamicArray, 7));
    dynamicArray.EmplaceBack(7);
    JPT_ENSURE(jpt::Has(dynamicArray, 7));

    *jpt::Find(dynamicArray, 7) = 8;
    JPT_ENSURE(!jpt::Has(dynamicArray, 7));
    JPT_ENSURE(jpt::Has(dynamicArray, 8));

    JPT_ENSURE(jpt::FindIf(dynamicArray, [](int32 n) {return n == 8; }) != dynamicArray.end());

    jpt::DynamicArray<jpt::String> dynamicStrArray = { "Zero", "One", "Two", "Three", "Four" };

    JPT_ENSURE(jpt::Find(dynamicStrArray, "Six") == dynamicStrArray.end());
    JPT_ENSURE(jpt::Has(dynamicStrArray, "Zero"));

    JPT_ENSURE(!jpt::Has(dynamicStrArray, "Seven"));
    dynamicStrArray.EmplaceBack("Seven");
    JPT_ENSURE(jpt::Has(dynamicStrArray, "Seven"));

    *jpt::Find(dynamicStrArray, "Seven") = "Eight";
    JPT_ENSURE(!jpt::Has(dynamicStrArray, "Seven"));
    JPT_ENSURE(jpt::Has(dynamicStrArray, "Eight"));

    JPT_ENSURE(jpt::FindIf(dynamicStrArray, [](const jpt::String& str) {return str == "Eight"; }) != dynamicStrArray.end());

    return true;
}

bool UnitTests_Searching_LinkedList()
{
    jpt::LinkedList<int32> linkedList = { 1, 2, 3, 4, 5 };

    JPT_ENSURE(jpt::Find(linkedList, 6) == linkedList.end());
    JPT_ENSURE(jpt::Has(linkedList, 5));

    JPT_ENSURE(!jpt::Has(linkedList, 7));
    linkedList.EmplaceBack(7);
    JPT_ENSURE(jpt::Has(linkedList, 7));

    *jpt::Find(linkedList, 7) = 8;
    JPT_ENSURE(!jpt::Has(linkedList, 7));
    JPT_ENSURE(jpt::Has(linkedList, 8));

    JPT_ENSURE(jpt::FindIf(linkedList, [](int32 n) {return n == 8; }) != linkedList.end());

    jpt::DynamicArray<jpt::String> linkedListStr = { "Zero", "One", "Two", "Three", "Four" };

    JPT_ENSURE(jpt::Find(linkedListStr, "Six") == linkedListStr.end());
    JPT_ENSURE(jpt::Has(linkedListStr, "Zero"));

    JPT_ENSURE(!jpt::Has(linkedListStr, "Seven"));
    linkedListStr.EmplaceBack("Seven");
    JPT_ENSURE(jpt::Has(linkedListStr, "Seven"));

    *jpt::Find(linkedListStr, "Seven") = "Eight";
    JPT_ENSURE(!jpt::Has(linkedListStr, "Seven"));
    JPT_ENSURE(jpt::Has(linkedListStr, "Eight"));

    JPT_ENSURE(jpt::FindIf(linkedListStr, [](const jpt::String& str) {return str == "Eight"; }) != linkedListStr.end());

    return true;
}

bool UnitTests_Searching_RawArray()
{
    int32 rawArray[] = { 1, 2, 3, 4, 5 };

    JPT_ENSURE(jpt::Find(rawArray, 1) == 0);
    JPT_ENSURE(jpt::Find(rawArray, 2) == 1);
    JPT_ENSURE(jpt::Find(rawArray, 3) == 2);
    JPT_ENSURE(jpt::Find(rawArray, 4) == 3);
    JPT_ENSURE(jpt::Find(rawArray, 5) == 4);
    JPT_ENSURE(jpt::Find(rawArray, 6) == jpt::kInvalidValue<size_t>);
    JPT_ENSURE(jpt::Find(rawArray, 0) == jpt::kInvalidValue<size_t>);

    return true;
}

export bool RunUnitTests_Searching()
{
    JPT_ENSURE(UnitTests_Searching_StaticArray());
    JPT_ENSURE(UnitTests_Searching_DynamicArray());
    JPT_ENSURE(UnitTests_Searching_LinkedList());
    JPT_ENSURE(UnitTests_Searching_RawArray());

    return true;
}
