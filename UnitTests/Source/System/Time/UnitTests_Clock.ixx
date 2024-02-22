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
import jpt.DateTime;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;

bool UnitTest_Clock()
{
    jpt::DateTime now = jpt::Clock::GetCurrentDateTime();
    jpt::DateTime tenMinsAgo = jpt::Clock::RawNow() - 60 * 50;
    jpt::DateTime tenMinsDuration = now - tenMinsAgo;

    JPT_LOG("10 minutes duration: " + tenMinsDuration.ToString());
    JPT_LOG("10 minutes later: "    + (now + tenMinsDuration).ToString());
    JPT_LOG("10 minutes before: "   + (now - tenMinsDuration).ToString());

    return true;
}

bool UnitTest_DateTime()
{    
    jpt::DateTime now = jpt::Clock::GetCurrentDateTime();

    // Log the time 10 years ago
    now.Backward(60 * 60 * 24 * 365 * 10);
    JPT_LOG("10 years ago: " + now.ToString());

    // Log the time 20 years later
    now.Forward(60 * 60 * 24 * 365 * 20);
    JPT_LOG("10 years later: " + now.ToString());

    return true;
}

export bool RunUnitTests_Clock()
{
    //JPT_LOG("Current Time: " + jpt::Clock::GetCurrentDateTime().ToString());
    //JPT_ENSURE(UnitTest_Clock());
    //JPT_ENSURE(UnitTest_DateTime());

    return true;
}
