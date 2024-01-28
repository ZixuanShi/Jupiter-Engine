// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

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

    JPT_LOG(jpt::Timing::GetSecondsBetween(start, end));

    return true;
}

bool UnitTest_StopWatch()
{
    jpt::Timing::Point start = jpt::Timing::Now<jpt::Timing::StopWatch>();
    Sleep(3'000);
    jpt::Timing::Point end = jpt::Timing::Now<jpt::Timing::StopWatch>();

    JPT_LOG(jpt::Timing::GetSecondsBetween(start, end));

    return true;
}

export bool RunUnitTests_Timing()
{
    //JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Clock(), "UnitTest_Clock Failed");
    //JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StopWatch(), "UnitTest_StopWatch Failed");

    return true;
}
