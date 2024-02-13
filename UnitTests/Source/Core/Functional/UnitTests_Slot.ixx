// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Slot;

import jpt.Slot;
import jpt.TypeDefs;

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

    // Call all
    slot(n);
    JPT_RETURN_FALSE_IF_ERROR(n != 64, "");
    
    return true;
}

export bool RunUnitTests_Slot()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Slot_Void(), "UnitTest_Slot Failed");

    return true;
}
