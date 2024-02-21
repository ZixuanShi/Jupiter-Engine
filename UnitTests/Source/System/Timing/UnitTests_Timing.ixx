// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <chrono>

#if IS_PLATFORM_WIN64
    #include <Windows.h>
#endif

export module UnitTests_Timing;

import jpt.Timing.Utils;
import jpt.Timing.TypeDefs;
import jpt.Math;
import jpt.Utilities;

bool UnitTest_Clock()
{
    jpt::TimePoint start = jpt::Now<jpt::Clock>();
    Sleep(1'000);
    jpt::TimePoint end = jpt::Now<jpt::Clock>();

    double secondsBetween = jpt::GetSecondsBetween(start, end);
    bool value = jpt::AreValuesClose(secondsBetween, 1.0, 0.1);
    JPT_ENSURE(value);

    secondsBetween = jpt::GetSecondsFrom(start);
    value = jpt::AreValuesClose(secondsBetween, 1.0, 0.1);
    JPT_ENSURE(value);

    return true;
}

bool UnitTest_StopWatch()
{
    jpt::TimePoint start = jpt::Now<jpt::StopWatch>();
    Sleep(1'000);
    jpt::TimePoint end = jpt::Now<jpt::StopWatch>();

    double secondsBetween = jpt::GetSecondsBetween(start, end);
    bool value = jpt::AreValuesClose(secondsBetween, 1.0, 0.1);
    JPT_ENSURE(value);

    secondsBetween = jpt::GetSecondsFrom(start);
    value = jpt::AreValuesClose(secondsBetween, 1.0, 0.1);
    JPT_ENSURE(value);

    return true;
}

export bool RunUnitTests_Timing()
{
    JPT_ENSURE(UnitTest_Clock());
    JPT_ENSURE(UnitTest_StopWatch());

    return true;
}
