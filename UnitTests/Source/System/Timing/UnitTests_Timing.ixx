// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <Windows.h>
#include <chrono>

export module UnitTests_Timing;

import jpt.CoreModules;
import jpt.Timing.Utils;

bool UnitTest_Clock()
{
    jpt::Timing::Point start = jpt::Timing::Now<jpt::Timing::Clock>();
    Sleep(3'000);
    jpt::Timing::Point end = jpt::Timing::Now<jpt::Timing::Clock>();

    const double secondsBetween = jpt::Timing::GetSecondsBetween(start, end);
    const bool value = jpt::AreValuesClose(secondsBetween, 3.0, 0.1);
    JPT_RETURN_FALSE_IF_ERROR(!value, "");

    return true;
}

bool UnitTest_StopWatch()
{
    jpt::Timing::Point start = jpt::Timing::Now<jpt::Timing::StopWatch>();
    Sleep(3'500);
    jpt::Timing::Point end = jpt::Timing::Now<jpt::Timing::StopWatch>();

    const double secondsBetween = jpt::Timing::GetSecondsBetween(start, end);
    const bool value = jpt::AreValuesClose(secondsBetween, 3.5, 0.1);
    JPT_RETURN_FALSE_IF_ERROR(!value, "");

    return true;
}

export bool RunUnitTests_Timing()
{
    //JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Clock(), "UnitTest_Clock Failed");
    //JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StopWatch(), "UnitTest_StopWatch Failed");

    return true;
}