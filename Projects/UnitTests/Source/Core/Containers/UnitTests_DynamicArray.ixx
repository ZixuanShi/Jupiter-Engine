// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <vector>

export module UnitTests_DynamicArray;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.String;
import jpt.DynamicArray;

auto locHelper = [](size_t i) -> const char*
    {
        switch (i)
        {
            case 0: return "Zero";
            case 1: return "One";
            case 2: return "Two";
            case 3: return "Three";
            case 4: return "Four";
            case 5: return "Five";
            case 6: return "Six";
            case 7: return "Seven";
            case 8: return "Eight";
            case 9: return "Nine";
            case 10: return "Ten";
            case 11: return "Eleven";
            case 12: return "Twelve";

            default: return "Error";
        }
    };

bool UnitTest_DynamicArray_Trivial()
{
    jpt::DynamicArray<int32> dynamicArray{ 0,2,4 };
    int32 i = 0;

    for (int32 element : dynamicArray)
    {
        JPT_ENSURE(element == i * 2);
        ++i;
    }

    // Inserting
    dynamicArray.Add(1, 1);
    int32 data = dynamicArray.Emplace(3, 3);
    JPT_ENSURE(data == 3);
    dynamicArray.AddBack(5);
    dynamicArray.AddBack(6);
    dynamicArray.AddBack(6);
    data = dynamicArray.EmplaceBack(6);
    JPT_ENSURE(data == 6);

    // Erasing
    dynamicArray.Erase(6);
    dynamicArray.PopBack();

    // iterating
    i = 0;
    for (int32 element : dynamicArray)
    {
        JPT_ENSURE(element == i);
        ++i;
    }

    for (int32& element : dynamicArray)
    {
        element *= 2;
    }

    i = 0;
    for (int32 element : dynamicArray)
    {
        JPT_ENSURE(element == i * 2);
        ++i;
    }

    // Capacity
    dynamicArray.Reserve(32);
    JPT_ENSURE(dynamicArray.Capacity() == 32);

    dynamicArray.ShrinkToFit();
    JPT_ENSURE(dynamicArray.Capacity() == 7);

    dynamicArray.Resize(16, 42);
    JPT_ENSURE(dynamicArray.Count() == 16);

    for (i = 7; i < 16; ++i)
    {
        JPT_ENSURE(dynamicArray[i] == 42);
    }

    // Copying
    jpt::DynamicArray<int32> copiedArray(dynamicArray);
    for (i = 7; i < 16; ++i)
    {
        JPT_ENSURE(copiedArray[i] == 42);
    }
    for (i = 7; i < 16; ++i)
    {
        JPT_ENSURE(dynamicArray[i] == 42);
    }

    copiedArray.Resize(7);
    dynamicArray = copiedArray;
    i = 0;
    for (int32 element : copiedArray)
    {
        JPT_ENSURE(element == i * 2);
        ++i;
    }
    i = 0;
    for (int32 element : dynamicArray)
    {
        JPT_ENSURE(element == i * 2);
        ++i;
    }

    // Moving
    jpt::DynamicArray<int32> movedArray(Move(dynamicArray));
    JPT_ENSURE(dynamicArray.ConstBuffer() == nullptr);
    JPT_ENSURE(dynamicArray.IsEmpty());
    JPT_ENSURE(dynamicArray.Capacity() == 0);
    i = 0;
    for (int32 element : movedArray)
    {
        JPT_ENSURE(element == i * 2);
        ++i;
    }

    copiedArray = Move(movedArray);
    JPT_ENSURE(movedArray.ConstBuffer() == nullptr);
    JPT_ENSURE(movedArray.IsEmpty());
    JPT_ENSURE(movedArray.Capacity() == 0);

    i = 0;
    for (int32 element : copiedArray)
    {
        JPT_ENSURE(element == i * 2);
        ++i;
    }
    //JPT_LOG(copiedArray);

    return true;
}

bool UnitTest_DynamicArray_NonTrivial()
{
    jpt::DynamicArray<jpt::String> dynamicArray{ "Zero", "Two", "Four" };
    size_t i = 0;

    for (const jpt::String& element : dynamicArray)
    {
        JPT_ENSURE(element == locHelper(i * 2));
        ++i;
    }

    // Inserting
    dynamicArray.Add(1, locHelper(1));
    const jpt::String& data1 = dynamicArray.Emplace(3, "Three");
    JPT_ENSURE(data1 == "Three");
    dynamicArray.AddBack(locHelper(5));
    dynamicArray.AddBack(locHelper(6));
    dynamicArray.AddBack(locHelper(6));
    const jpt::String& data2 = dynamicArray.EmplaceBack("Six");
    JPT_ENSURE(data2 == "Six");

    // Erasing
    dynamicArray.Erase(6);
    dynamicArray.PopBack();

    // Iterating
    i = 0;
    for (const jpt::String& element : dynamicArray)
    {
        JPT_ENSURE(element == locHelper(i));
        ++i;
    }

    i = 0;
    for (jpt::String& element : dynamicArray)
    {
        element = locHelper(2 * i);
        ++i;
    }

    i = 0;
    for (const jpt::String& element : dynamicArray)
    {
        JPT_ENSURE(element == locHelper(i * 2));
        ++i;
    }

    // Capacity
    dynamicArray.Reserve(32);
    JPT_ENSURE(dynamicArray.Capacity() == 32);

    dynamicArray.ShrinkToFit();
    JPT_ENSURE(dynamicArray.Capacity() == 7);

    dynamicArray.Resize(16, "42");
    JPT_ENSURE(dynamicArray.Count() == 16);

    for (i = 7; i < 16; ++i)
    {
        JPT_ENSURE(dynamicArray[i] == "42");
    }

    // Copying
    jpt::DynamicArray<jpt::String> copiedArray(dynamicArray);
    for (i = 7; i < 16; ++i)
    {
        JPT_ENSURE(copiedArray[i] == "42");
    }
    for (i = 7; i < 16; ++i)
    {
        JPT_ENSURE(dynamicArray[i] == "42");
    }

    copiedArray.Resize(7);
    dynamicArray = copiedArray;
    i = 0;
    for (const jpt::String& element : copiedArray)
    {
        JPT_ENSURE(element == locHelper(i * 2));
        ++i;
    }
    i = 0;
    for (const jpt::String& element : dynamicArray)
    {
        JPT_ENSURE(element == locHelper(i * 2));
        ++i;
    }

    // Moving
    jpt::DynamicArray<jpt::String> movedArray(Move(dynamicArray));
    JPT_ENSURE(dynamicArray.ConstBuffer() == nullptr);
    JPT_ENSURE(dynamicArray.IsEmpty());
    JPT_ENSURE(dynamicArray.Capacity() == 0);
    i = 0;
    for (const jpt::String& element : movedArray)
    {
        JPT_ENSURE(element == locHelper(i * 2));
        ++i;
    }

    copiedArray = Move(movedArray);
    JPT_ENSURE(movedArray.ConstBuffer() == nullptr);
    JPT_ENSURE(movedArray.IsEmpty());
    JPT_ENSURE(movedArray.Capacity() == 0);

    i = 0;
    for (const jpt::String& element : copiedArray)
    {
        JPT_ENSURE(element == locHelper(i * 2));
        ++i;
    }
    //JPT_LOG(copiedArray);

    return true;
}

export bool RunUnitTests_DynamicArray()
{
    JPT_ENSURE(UnitTest_DynamicArray_Trivial());
    JPT_ENSURE(UnitTest_DynamicArray_NonTrivial());

    return true;
}
