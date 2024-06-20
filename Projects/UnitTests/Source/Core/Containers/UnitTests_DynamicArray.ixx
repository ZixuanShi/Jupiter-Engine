// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <vector>

export module UnitTests_DynamicArray;

import jpt.Any;
import jpt.DynamicArray;
import jpt.String;
import jpt.TypeDefs;
import jpt.Tuple;
import jpt.Utilities;
import jpt.Variant;

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
    dynamicArray.Add(5);
    dynamicArray.Add(6);
    dynamicArray.Add(6);
    data = dynamicArray.EmplaceBack(6);
    JPT_ENSURE(data == 6);

    // Erasing
    dynamicArray.Erase(6);
    dynamicArray.Pop();

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
    dynamicArray.Add(locHelper(5));
    dynamicArray.Add(locHelper(6));
    dynamicArray.Add(locHelper(6));
    const jpt::String& data2 = dynamicArray.EmplaceBack("Six");
    JPT_ENSURE(data2 == "Six");

    // Erasing
    dynamicArray.Erase(6);
    dynamicArray.Pop();

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

JPT_ENUM_UINT32(ETest, 
    Zero = 12, 
    One, 
    Two, 
    Three, 
    Four = (1 << 8),
    Five, 
    Six);
bool UnitTest_DynamicArray_Enum()
{
    jpt::DynamicArray<ETest> dynamicArray{ ETest::Zero, ETest::Two, ETest::Four };
    
    JPT_ENSURE(dynamicArray[0] == ETest::Zero);
    JPT_ENSURE(dynamicArray[0].Value() == 12);
    JPT_ENSURE(dynamicArray[0].ToString() == "Zero");

    JPT_ENSURE(dynamicArray[1] == ETest::Two);
    JPT_ENSURE(dynamicArray[1].Value() == 14);
    JPT_ENSURE(dynamicArray[1].ToString() == "Two");

    JPT_ENSURE(dynamicArray[2] == ETest::Four);
    JPT_ENSURE(dynamicArray[2].Value() == 256);
    JPT_ENSURE(dynamicArray[2].ToString() == "Four");

    dynamicArray[1] = ETest::Six;
    JPT_ENSURE(dynamicArray[1] == ETest::Six);
    JPT_ENSURE(dynamicArray[1].Value() == 258);
    JPT_ENSURE(dynamicArray[1].ToString() == "Six");

    return true;
}

bool UnitTest_DynamicArray_Any()
{
    jpt::DynamicArray<jpt::Any> dynamicArray{ 1, 2.0f, jpt::String("Four"), jpt::Tuple<int32, bool>(5, false), jpt::Variant<int32>(7) };

    JPT_ENSURE(dynamicArray[0].Is<int32>());
    JPT_ENSURE(dynamicArray[0].As<int32>() == 1);
    dynamicArray[0] = 2;
    JPT_ENSURE(dynamicArray[0].As<int32>() == 2);

    JPT_ENSURE(dynamicArray[1].Is<float>());
    JPT_ENSURE(dynamicArray[1].As<float>() == 2.0f);
    dynamicArray[1] = 3.0f;
    JPT_ENSURE(dynamicArray[1].As<float>() == 3.0f);

    JPT_ENSURE(dynamicArray[2].Is<jpt::String>());
    JPT_ENSURE(dynamicArray[2].As<jpt::String>() == "Four");
    dynamicArray[2] = jpt::String("Five");
    JPT_ENSURE(dynamicArray[2].As<jpt::String>() == "Five");

    JPT_ENSURE((dynamicArray[3].Is<jpt::Tuple<int32, bool>>()));
    JPT_ENSURE(jpt::Get<0>(dynamicArray[3].As<jpt::Tuple<int32, bool>>()) == 5);
    JPT_ENSURE(jpt::Get<1>(dynamicArray[3].As<jpt::Tuple<int32, bool>>()) == false);
    dynamicArray[3] = jpt::Tuple<int32, bool>(6, true);
    JPT_ENSURE(jpt::Get<0>(dynamicArray[3].As<jpt::Tuple<int32, bool>>()) == 6);
    JPT_ENSURE(jpt::Get<1>(dynamicArray[3].As<jpt::Tuple<int32, bool>>()) == true);

    JPT_ENSURE(dynamicArray[4].Is<jpt::Variant<int32>>());
    JPT_ENSURE(dynamicArray[4].As<jpt::Variant<int32>>().Is<int32>());
    JPT_ENSURE(dynamicArray[4].As<jpt::Variant<int32>>().As<int32>() == 7);
    dynamicArray[4] = jpt::Variant<int32>(8);
    JPT_ENSURE(dynamicArray[4].As<jpt::Variant<int32>>().As<int32>() == 8);

    return true;
}

bool UnitTest_DynamicArray_Tuple()
{
    using TTuple = jpt::Tuple<int32, float, jpt::String, jpt::Variant<int32, char, bool>>;
    jpt::DynamicArray<TTuple> dynamicArray;

    dynamicArray.EmplaceBack(1, 2.0f, jpt::String("Four"), jpt::Variant<int32, char, bool>(5));
    dynamicArray.EmplaceBack(2, 3.0f, jpt::String("Five"), jpt::Variant<int32, char, bool>('6'));
    dynamicArray.EmplaceBack(3, 4.0f, jpt::String("Six"), jpt::Variant<int32, char, bool>(true));

    JPT_ENSURE(jpt::Get<0>(dynamicArray[0]) == 1);
    JPT_ENSURE(jpt::Get<1>(dynamicArray[0]) == 2.0f);
    JPT_ENSURE(jpt::Get<2>(dynamicArray[0]) == "Four");
    JPT_ENSURE(jpt::Get<3>(dynamicArray[0]).Is<int32>());
    JPT_ENSURE(jpt::Get<3>(dynamicArray[0]).As<int32>() == 5);

    JPT_ENSURE(jpt::Get<0>(dynamicArray[1]) == 2);
    JPT_ENSURE(jpt::Get<1>(dynamicArray[1]) == 3.0f);
    JPT_ENSURE(jpt::Get<2>(dynamicArray[1]) == "Five");
    JPT_ENSURE(jpt::Get<3>(dynamicArray[1]).Is<char>());
    JPT_ENSURE(jpt::Get<3>(dynamicArray[1]).As<char>() == '6');

    JPT_ENSURE(jpt::Get<0>(dynamicArray[2]) == 3);
    JPT_ENSURE(jpt::Get<1>(dynamicArray[2]) == 4.0f);
    JPT_ENSURE(jpt::Get<2>(dynamicArray[2]) == "Six");
    JPT_ENSURE(jpt::Get<3>(dynamicArray[2]).Is<bool>());
    JPT_ENSURE(jpt::Get<3>(dynamicArray[2]).As<bool>() == true);

    jpt::Get<0>(dynamicArray[0]) = 10;
    jpt::Get<1>(dynamicArray[0]) = 20.0f;
    jpt::Get<2>(dynamicArray[0]) = jpt::String("Ten");
    jpt::Get<3>(dynamicArray[0]) = jpt::Variant<int32, char, bool>(15);
    JPT_ENSURE(jpt::Get<0>(dynamicArray[0]) == 10);
    JPT_ENSURE(jpt::Get<1>(dynamicArray[0]) == 20.0f);
    JPT_ENSURE(jpt::Get<2>(dynamicArray[0]) == "Ten");
    JPT_ENSURE(jpt::Get<3>(dynamicArray[0]).Is<int32>());

    return true;
}

bool UnitTest_DynamicArray_Variant()
{
    using TVariant = jpt::Variant<int32, float, jpt::String, jpt::Tuple<int32, char, bool>>;
    jpt::DynamicArray<TVariant> dynamicArray;

    dynamicArray.EmplaceBack(1);
    dynamicArray.EmplaceBack(2.0f);
    dynamicArray.EmplaceBack(jpt::String("Four"));
    dynamicArray.EmplaceBack(jpt::Tuple<int32, char, bool>(5, '6', true));

    JPT_ENSURE(dynamicArray[0].Is<int32>());
    JPT_ENSURE(dynamicArray[0].As<int32>() == 1);

    JPT_ENSURE(dynamicArray[1].Is<float>());
    JPT_ENSURE(dynamicArray[1].As<float>() == 2.0f);

    JPT_ENSURE(dynamicArray[2].Is<jpt::String>());
    JPT_ENSURE(dynamicArray[2].As<jpt::String>() == "Four");

    JPT_ENSURE((dynamicArray[3].Is<jpt::Tuple<int32, char, bool>>()));
    JPT_ENSURE(jpt::Get<0>(dynamicArray[3].As<jpt::Tuple<int32, char, bool>>()) == 5);

    dynamicArray[0] = 10;
    dynamicArray[1] = 20.0f;
    dynamicArray[2] = jpt::String("Ten");
    dynamicArray[3] = jpt::Tuple<int32, char, bool>(15, '6', false);

    JPT_ENSURE(dynamicArray[0].Is<int32>());
    JPT_ENSURE(dynamicArray[0].As<int32>() == 10);

    JPT_ENSURE(dynamicArray[1].Is<float>());
    JPT_ENSURE(dynamicArray[1].As<float>() == 20.0f);

    JPT_ENSURE(dynamicArray[2].Is<jpt::String>());
    JPT_ENSURE(dynamicArray[2].As<jpt::String>() == "Ten");

    JPT_ENSURE((dynamicArray[3].Is<jpt::Tuple<int32, char, bool>>()));
    JPT_ENSURE(jpt::Get<0>(dynamicArray[3].As<jpt::Tuple<int32, char, bool>>()) == 15);

    return true;
}

export bool RunUnitTests_DynamicArray()
{
    JPT_ENSURE(UnitTest_DynamicArray_Trivial());
    JPT_ENSURE(UnitTest_DynamicArray_NonTrivial());

    JPT_ENSURE(UnitTest_DynamicArray_Enum());
    JPT_ENSURE(UnitTest_DynamicArray_Any());
    JPT_ENSURE(UnitTest_DynamicArray_Tuple());
    JPT_ENSURE(UnitTest_DynamicArray_Variant());

    return true;
}
