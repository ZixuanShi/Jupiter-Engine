// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_StaticArray;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.StaticArray;
import jpt.String;

auto locHelper = [](size_t i) -> const char*
    {
        switch (i)
        {
        case 0: return "Zero";
        case 1: return "One";
        case 2: return "Two";
        case 3: return "Three";
        case 4: return "Four";

        default: return "Error";
        }
    };

bool UnitTest_StaticArray_Constructing_Trivial()
{
    jpt::StaticArray<int32, 5> staticArray{ 0,1,2,3,4 };

    int32 i = 0;
    for (int32 element : staticArray)
    {
        JPT_ENSURE(element == i);
        ++i;
    }

    for (int32& element : staticArray)
    {
        element *= 2;
    }

    i = 0;
    for (int32 element : staticArray)
    {
        JPT_ENSURE(element == i * 2);
        ++i;
    }

    return true;
}

bool UnitTest_StaticArray_Constructing_NonTrivial()
{
    jpt::StaticArray<jpt::String, 5> staticArray{ "Zero", "One", "Two", "Three", "Four" };

    size_t i = 0;
    for (const jpt::String& element : staticArray)
    {
        JPT_ENSURE(element == locHelper(i++));
    }

    for (i = 0; i < staticArray.Count() / 2; ++i)
    {
        jpt::Swap(staticArray[i], staticArray[staticArray.Count() - 1 - i]);
    }

    i = staticArray.Count() - 1;
    for (const jpt::String& element : staticArray)
    {
        JPT_ENSURE(element == locHelper(i--));
    }

    return true;
}

bool UnitTest_StaticArray_Copying_Trivial()
{
    jpt::StaticArray<int32, 5> staticArray1{ 9,8,7,6,5 };
    jpt::StaticArray<int32, 5> staticArray2{ 0,1,2,3,4 };

    staticArray1 = staticArray2;

    int32 i = 0;
    for (int32 element : staticArray1)
    {
        JPT_ENSURE(element == i);
        ++i;
    }

    return true;
}

bool UnitTest_StaticArray_Copying_NonTrivial()
{
    jpt::StaticArray<jpt::String, 5> staticArray1{ "Four", "Three", "Two", "One", "Zero" };
    jpt::StaticArray<jpt::String, 5> staticArray2{ "Zero", "One", "Two", "Three", "Four" };

    staticArray1 = staticArray2;

    size_t i = 0;
    for (const jpt::String& element : staticArray1)
    {
        JPT_ENSURE(element == locHelper(i++));
    }
    
    return true;
}

bool UnitTest_StaticArray_Moving()
{
    jpt::StaticArray<jpt::String, 5> staticArray1{ "Four", "Three", "Two", "One", "Zero" };
    jpt::StaticArray<jpt::String, 5> staticArray2{ "Zero", "One", "Two", "Three", "Four" };

    jpt::Swap(staticArray1, staticArray2);

    size_t i = 0;
    for (const jpt::String& element : staticArray1)
    {
        JPT_ENSURE(element == locHelper(i++));
    }

    //JPT_LOG(staticArray1);

    return true;
}

bool UnitTest_StaticArray_HeapAllocating()
{
    jpt::StaticArray<jpt::String*, 5> staticArray1{ new jpt::String("Four"), new jpt::String("Three"), new jpt::String("Two"), new jpt::String("One"), new jpt::String("Zero") };
    jpt::StaticArray<jpt::String*, 5> staticArray2{ new jpt::String("Zero"), new jpt::String("One"), new jpt::String("Two"), new jpt::String("Three"), new jpt::String("Four") };

    jpt::Swap(staticArray1, staticArray2);

    size_t i = 0;
    for (const jpt::String* element : staticArray1)
    {
        JPT_ENSURE(*element == locHelper(i++));
    }

    for (jpt::String* element : staticArray1)
    {
        JPT_DELETE(element);
    }
    for (jpt::String* element : staticArray2)
    {
        JPT_DELETE(element);
    }

    return true;
}

export bool RunUnitTests_StaticArray()
{
    JPT_ENSURE(UnitTest_StaticArray_Constructing_Trivial());
    JPT_ENSURE(UnitTest_StaticArray_Constructing_NonTrivial());
    JPT_ENSURE(UnitTest_StaticArray_Copying_Trivial());
    JPT_ENSURE(UnitTest_StaticArray_Copying_NonTrivial());
    JPT_ENSURE(UnitTest_StaticArray_Moving());
    JPT_ENSURE(UnitTest_StaticArray_HeapAllocating());

    return true;
}
