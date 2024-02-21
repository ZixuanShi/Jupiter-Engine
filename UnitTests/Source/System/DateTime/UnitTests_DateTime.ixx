// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <time.h>
#include <chrono>

export module UnitTests_DateTime;

import jpt.DateTime;
import jpt.Timing.Utils;
import jpt.Timing.TypeDefs;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_DateTime()
{
    auto now = jpt::GetCurrentDateTime();
    JPT_LOG(now);

    now.year += 100;
    now.second -= 60;
    JPT_LOG(now);

    jpt::Timing::Point current = jpt::Timing::Now();
    JPT_LOG(jpt::Timing::GetSecondsFrom(current));

    return true;
}

export bool RunUnitTests_DateTime()
{
    JPT_ENSURE(UnitTest_DateTime());

    return true;
}
