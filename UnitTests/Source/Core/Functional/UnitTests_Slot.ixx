// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Slot;

import jpt.Slot;

void VoidFunc()
{
    JPT_LOG("VoidFunc");
}

bool UnitTest_Slot_Void()
{
    jpt::Slot<void()> slot;

    auto lambda = []() { JPT_LOG("Lambda"); };
    slot.Add(lambda);
    slot.Add([]() { JPT_LOG("Void Lambda placeholder"); });
    slot.Add(VoidFunc);
    slot.Erase(lambda);

    slot();

    return true;
}

export bool RunUnitTests_Slot()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Slot_Void(), "UnitTest_Slot Failed");

    return true;
}
