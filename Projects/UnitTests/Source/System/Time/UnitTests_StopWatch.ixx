// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

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

bool UnitTests_StopWatch()
{
#if IS_PLATFORM_WIN64
    const jpt::StopWatch::Point start = jpt::StopWatch::Now();

    Sleep(1000);
    
    const jpt::StopWatch::Point end = jpt::StopWatch::Now();

    JPT_LOG(jpt::StopWatch::GetSecondsBetween(start, end));
    JPT_LOG(jpt::StopWatch::GetSecondsFrom(start));
    JPT_LOG(jpt::StopWatch::GetMsBetween(start, end));
    JPT_LOG(jpt::StopWatch::GetMsFrom(start));
#endif

    return true;
}

bool UnitTests_Timer()
{
#if IS_PLATFORM_WIN64
    jpt::StopWatch timer;
    timer.Start();

    Sleep(1000);

    JPT_ENSURE(jpt::AreValuesClose(timer.GetDuration(), 1.0f, 0.1f));
    JPT_ENSURE(jpt::AreValuesClose(timer.GetDurationMs(), 1000.0f, 100.0f));
#endif

    return true;
}

export bool RunUnitTests_StopWatch()
{
    //JPT_ENSURE(UnitTests_StopWatch());
    //JPT_ENSURE(UnitTests_Timer());

    return true;
}
