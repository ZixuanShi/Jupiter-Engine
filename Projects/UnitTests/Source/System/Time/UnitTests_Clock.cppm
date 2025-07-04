// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

#include <time.h>
#include <chrono>

export module UnitTests_Clock;

import jpt.Clock;
import jpt.DateTime;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Math;

bool UnitTests_Clock()
{
    jpt::DateTime now = jpt::Clock::GetCurrentDateTime();
    jpt::DateTime ago = jpt::Clock::RawNow() - 60 * 50;
    jpt::DateTime duration = now - ago;

    JPT_LOG("50 minutes duration: " + jpt::ToString(duration));
    JPT_LOG("50 minutes later: "    + jpt::ToString((now + duration)));
    JPT_LOG("50 minutes before: "   + jpt::ToString((now - duration)));

    return true;
}

bool UnitTests_DateTime()
{    
    jpt::DateTime now = jpt::Clock::GetCurrentDateTime();

    // Log the time 10 years ago
    now.Backward(60 * 60 * 24 * 365 * 10);
    JPT_LOG("10 years ago: " + jpt::ToString(now));

    // Log the time 20 years later
    now.Forward(60 * 60 * 24 * 365 * 20);
    JPT_LOG("10 years later: " + jpt::ToString(now));

    return true;
}

export bool RunUnitTests_Clock()
{
    //JPT_LOG("Current Time: " + jpt::Clock::GetCurrentDateTime().ToString());
    //JPT_ENSURE(UnitTests_Clock());
    //JPT_ENSURE(UnitTests_DateTime());

    return true;
}
