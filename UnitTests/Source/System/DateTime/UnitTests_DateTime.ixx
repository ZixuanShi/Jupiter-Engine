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
    
    auto cu = jpt::Now();
    time_t current = time(nullptr);
    time_t tenMinsAgo = current - 10 * 60;

    char ctimeStr[64];
    ctime_s(ctimeStr, 64, &tenMinsAgo);
    JPT_LOG(ctimeStr);

    return true;
}

export bool RunUnitTests_DateTime()
{
    JPT_ENSURE(UnitTest_DateTime());

    return true;
}
