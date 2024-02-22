// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <time.h>
#include <chrono>

#if IS_PLATFORM_WIN64
#include <Windows.h>
#endif

export module UnitTests_Clock;

import jpt.Clock;
import jpt.Time.Utils;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;

bool UnitTest_Clock()
{
    const jpt::Clock::Point start = jpt::Clock::Now();
    Sleep(1000);
    const jpt::Clock::Point end = jpt::Clock::Now();

    JPT_ENSURE(jpt::AreValuesClose(jpt::GetSecondsBetween(start, end), 1.0, 0.1));
    JPT_ENSURE(jpt::AreValuesClose(jpt::GetSecondsFrom(start), 1.0, 0.1));
    JPT_ENSURE(jpt::AreValuesClose(jpt::GetMsBetween(start, end), 1000.0, 100.0));
    JPT_ENSURE(jpt::AreValuesClose(jpt::GetMsFrom(start), 1000.0, 100.0));

    return true;
}

bool UnitTest_DateTime()
{

    return true;
}

export bool RunUnitTests_Clock()
{
    //JPT_ENSURE(UnitTest_Clock());
    JPT_ENSURE(UnitTest_DateTime());

    return true;
}
