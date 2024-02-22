// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <time.h>
#include <chrono>

#if IS_PLATFORM_WIN64
#include <Windows.h>
#endif

export module UnitTests_StopWatch;

import jpt.StopWatch;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;

bool UnitTest_StopWatch()
{
    const jpt::StopWatch::Point start = jpt::StopWatch::Now();
    Sleep(1000);
    const jpt::StopWatch::Point end = jpt::StopWatch::Now();

    JPT_LOG(jpt::StopWatch::GetSecondsBetween(start, end));
    JPT_LOG(jpt::StopWatch::GetSecondsFrom(start));
    JPT_LOG(jpt::StopWatch::GetMsBetween(start, end));
    JPT_LOG(jpt::StopWatch::GetMsFrom(start));

    return true;
}

bool UnitTest_Timer()
{
    jpt::StopWatch timer;
    timer.Start();

    Sleep(1000);

    JPT_ENSURE(jpt::AreValuesClose(timer.GetDuration(), 1.0, 0.1));
    JPT_ENSURE(jpt::AreValuesClose(timer.GetDurationMs(), 1000.0, 100.0));

    return true;
}

export bool RunUnitTests_StopWatch()
{
    //JPT_ENSURE(UnitTest_StopWatch());
    //JPT_ENSURE(UnitTest_Timer());

    return true;
}
