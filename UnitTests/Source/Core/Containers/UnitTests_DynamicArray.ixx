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
        JPT_RETURN_FALSE_IF_ERROR(element != i * 2, "");
        ++i;
    }

    dynamicArray.Insert(1, 1);
    int32 data = dynamicArray.Emplace(3, 3);
    JPT_RETURN_FALSE_IF_ERROR(data != 3, "");
    dynamicArray.PushBack(5);
    dynamicArray.PushBack(6);
    dynamicArray.PushBack(6);
    data = dynamicArray.EmplaceBack(6);
    JPT_RETURN_FALSE_IF_ERROR(data != 6, "");

    dynamicArray.Erase(6);
    dynamicArray.PopBack();

    i = 0;
    for (int32 element : dynamicArray)
    {
        JPT_RETURN_FALSE_IF_ERROR(element != i, "");
        ++i;
    }

    for (int32& element : dynamicArray)
    {
        element *= 2;
    }

    i = 0;
    for (int32 element : dynamicArray)
    {
        JPT_RETURN_FALSE_IF_ERROR(element != i * 2, "");
        ++i;
    }

    dynamicArray.Reserve(32);
    JPT_RETURN_FALSE_IF_ERROR(dynamicArray.Capacity() != 32, "");

    dynamicArray.ShrinkToFit();
    JPT_RETURN_FALSE_IF_ERROR(dynamicArray.Capacity() != 7, "");

    dynamicArray.Resize(16, 42);
    JPT_RETURN_FALSE_IF_ERROR(dynamicArray.Size() != 16, "");

    for (i = 7; i < 16; ++i)
    {
        JPT_RETURN_FALSE_IF_ERROR(dynamicArray[i] != 42, "");
    }

    // Copying
    jpt::DynamicArray<int32> copiedArray(dynamicArray);
    for (i = 7; i < 16; ++i)
    {
        JPT_RETURN_FALSE_IF_ERROR(copiedArray[i] != 42, "");
    }
    for (i = 7; i < 16; ++i)
    {
        JPT_RETURN_FALSE_IF_ERROR(dynamicArray[i] != 42, "");
    }

    copiedArray.Resize(7);
    dynamicArray = copiedArray;
    i = 0;
    for (int32 element : copiedArray)
    {
        JPT_RETURN_FALSE_IF_ERROR(element != i * 2, "");
        ++i;
    }
    i = 0;
    for (int32 element : dynamicArray)
    {
        JPT_RETURN_FALSE_IF_ERROR(element != i * 2, "");
        ++i;
    }

    // Moving
    jpt::DynamicArray<int32> movedArray(Move(dynamicArray));
    JPT_RETURN_FALSE_IF_ERROR(dynamicArray.ConstBuffer() != nullptr, "");
    JPT_RETURN_FALSE_IF_ERROR(!dynamicArray.IsEmpty(), "");
    JPT_RETURN_FALSE_IF_ERROR(dynamicArray.Capacity() != 0, "");
    i = 0;
    for (int32 element : movedArray)
    {
        JPT_RETURN_FALSE_IF_ERROR(element != i * 2, "");
        ++i;
    }

    copiedArray = Move(movedArray);
    JPT_RETURN_FALSE_IF_ERROR(movedArray.ConstBuffer() != nullptr, "");
    JPT_RETURN_FALSE_IF_ERROR(!movedArray.IsEmpty(), "");
    JPT_RETURN_FALSE_IF_ERROR(movedArray.Capacity() != 0, "");

    i = 0;
    for (int32 element : copiedArray)
    {
        JPT_RETURN_FALSE_IF_ERROR(element != i * 2, "");
        ++i;
    }

    return true;
}

bool UnitTest_DynamicArray_NonTrivial()
{
    jpt::DynamicArray<jpt::String> dynamicArray{ "Zero", "Two", "Four" };
    size_t i = 0;

    for (const jpt::String& element : dynamicArray)
    {
        JPT_RETURN_FALSE_IF_ERROR(element != locHelper(i * 2), "");
        ++i;
    }

    dynamicArray.Insert(1, locHelper(1));
    const jpt::String& data1 = dynamicArray.Emplace(3, "Three");
    JPT_RETURN_FALSE_IF_ERROR(data1 != "Three", "");
    dynamicArray.PushBack(locHelper(5));
    dynamicArray.PushBack(locHelper(6));
    dynamicArray.PushBack(locHelper(6));
    const jpt::String& data2 = dynamicArray.EmplaceBack("Six");
    JPT_RETURN_FALSE_IF_ERROR(data2 != "Six", "");

    dynamicArray.Erase(6);
    dynamicArray.PopBack();

    i = 0;
    for (const jpt::String& element : dynamicArray)
    {
        JPT_RETURN_FALSE_IF_ERROR(element != locHelper(i), "");
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
        JPT_RETURN_FALSE_IF_ERROR(element != locHelper(i * 2), "");
        ++i;
    }

    dynamicArray.Reserve(32);
    JPT_RETURN_FALSE_IF_ERROR(dynamicArray.Capacity() != 32, "");

    dynamicArray.ShrinkToFit();
    JPT_RETURN_FALSE_IF_ERROR(dynamicArray.Capacity() != 7, "");

    dynamicArray.Resize(16, "42");
    JPT_RETURN_FALSE_IF_ERROR(dynamicArray.Size() != 16, "");

    for (i = 7; i < 16; ++i)
    {
        JPT_RETURN_FALSE_IF_ERROR(dynamicArray[i] != "42", "");
    }

    // Copying
    jpt::DynamicArray<jpt::String> copiedArray(dynamicArray);
    for (i = 7; i < 16; ++i)
    {
        JPT_RETURN_FALSE_IF_ERROR(copiedArray[i] != "42", "");
    }
    for (i = 7; i < 16; ++i)
    {
        JPT_RETURN_FALSE_IF_ERROR(dynamicArray[i] != "42", "");
    }

    copiedArray.Resize(7);
    dynamicArray = copiedArray;
    i = 0;
    for (const jpt::String& element : copiedArray)
    {
        JPT_RETURN_FALSE_IF_ERROR(element != locHelper(i * 2), "");
        ++i;
    }
    i = 0;
    for (const jpt::String& element : dynamicArray)
    {
        JPT_RETURN_FALSE_IF_ERROR(element != locHelper(i * 2), "");
        ++i;
    }

    // Moving
    jpt::DynamicArray<jpt::String> movedArray(Move(dynamicArray));
    JPT_RETURN_FALSE_IF_ERROR(dynamicArray.ConstBuffer() != nullptr, "");
    JPT_RETURN_FALSE_IF_ERROR(!dynamicArray.IsEmpty(), "");
    JPT_RETURN_FALSE_IF_ERROR(dynamicArray.Capacity() != 0, "");
    i = 0;
    for (const jpt::String& element : movedArray)
    {
        JPT_RETURN_FALSE_IF_ERROR(element != locHelper(i * 2), "");
        ++i;
    }

    copiedArray = Move(movedArray);
    JPT_RETURN_FALSE_IF_ERROR(movedArray.ConstBuffer() != nullptr, "");
    JPT_RETURN_FALSE_IF_ERROR(!movedArray.IsEmpty(), "");
    JPT_RETURN_FALSE_IF_ERROR(movedArray.Capacity() != 0, "");

    i = 0;
    for (const jpt::String& element : copiedArray)
    {
        JPT_RETURN_FALSE_IF_ERROR(element != locHelper(i * 2), "");
        ++i;
    }

    return true;
}

export bool RunUnitTests_DynamicArray()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_DynamicArray_Trivial(), "UnitTest_DynamicArray_Trivial Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_DynamicArray_NonTrivial(), "UnitTest_DynamicArray_NonTrivial Failed");

    return true;
}
