// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Slot;

import jpt.Slot;
import jpt.TypeDefs;
import jpt.Variant;
import jpt.DynamicArray;
import jpt.ToString;

void VoidFunc(int32& n)
{
    n *= 2;
}

void Tester(int32& n)
{
    n *= 2;
}

bool UnitTest_Slot_Void()
{
    jpt::Slot<void(int32&)> slot;

    int32 n = 1;

    // Function pointer
    size_t voidFuncIndex = slot.Add(VoidFunc);
    slot.Call(voidFuncIndex, n);
    JPT_RETURN_FALSE_IF_ERROR(n != 2, "");

    // Lambda placeholder
    size_t lambdaPlaceHolderIndex = slot.Add([](int32& n) {n *= 2; });
    slot.Call(lambdaPlaceHolderIndex, n);
    JPT_RETURN_FALSE_IF_ERROR(n != 4, "");

    // Lambda variable
    auto lambda = [](int32& n) { n *= 2; };
    size_t lambdaVarIndex = slot.Add(lambda);
    slot.Call(lambdaVarIndex, n);
    JPT_RETURN_FALSE_IF_ERROR(n != 8, "");

    // Erasing
    slot.Erase(voidFuncIndex);  // index
    JPT_RETURN_FALSE_IF_ERROR(!slot.Erase(lambda), "");         // function pointer/lambda
    JPT_RETURN_FALSE_IF_ERROR(slot.Erase(Tester) != false, "");

    // Re-adding. the indices should be the same as before
    voidFuncIndex = slot.Add(VoidFunc);
    lambdaVarIndex = slot.Add(lambda);
    JPT_RETURN_FALSE_IF_ERROR(voidFuncIndex != 0, "");
    JPT_RETURN_FALSE_IF_ERROR(lambdaVarIndex != 2, "");

    slot.Erase(lambdaPlaceHolderIndex);
    jpt::Function<void(int32&)> function = VoidFunc;
    size_t functionIndex = slot.Add(function);
    JPT_RETURN_FALSE_IF_ERROR(functionIndex != 1, "");

    // Call all
    slot(n);
    JPT_RETURN_FALSE_IF_ERROR(n != 64, "");
    
    return true;
}

int32 CharToInt(char c)
{
    return c - '0';
}

int32 CharToIntTimes2(char c)
{
    return (c - '0') * 2;
}

bool UnitTest_Slot_Return()
{
    jpt::Slot<int32(char)> slot;

    size_t charToIntHandle        = slot.Add(CharToInt);
    size_t charToIntTimes2Handle  = slot.Add(CharToIntTimes2);
    size_t charToIntSquaredHandle = slot.Add([](char c) -> int32 {  return (c - '0') * (c - '0'); });
    
    jpt::DynamicArray<int32> results = slot.ReturnAll('3');

    JPT_RETURN_FALSE_IF_ERROR(results[charToIntHandle]        != 3, "");
    JPT_RETURN_FALSE_IF_ERROR(results[charToIntTimes2Handle]  != 6, "");
    JPT_RETURN_FALSE_IF_ERROR(results[charToIntSquaredHandle] != 9, "");

    return true;
}

using TVariant = jpt::Variant<int32, bool, char>;

void SetVariant(TVariant& variant)
{
    variant = false;
}

bool UnitTest_Slot_Variant()
{
    jpt::Slot<void(TVariant&)> slot;

    slot.Add(SetVariant);
    slot.Add([](TVariant& variant) 
        {
            variant.As<bool>() = true;
        });

    TVariant variant;
    slot(variant);

    JPT_RETURN_FALSE_IF_ERROR(!variant.Is<bool>(), "");
    JPT_RETURN_FALSE_IF_ERROR(!variant.As<bool>(), "");

    JPT_RETURN_FALSE_IF_ERROR(jpt::ToString(variant.As<bool>()) != "true", "");

    return true;
}

export bool RunUnitTests_Slot()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Slot_Void(), "UnitTest_Slot Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Slot_Return(), "UnitTest_Slot_Return Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Slot_Variant(), "UnitTest_Slot_Variant Failed");

    return true;
}
