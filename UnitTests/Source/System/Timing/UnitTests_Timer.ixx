// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <time.h>
#include <chrono>

#if IS_PLATFORM_WIN64
#include <Windows.h>
#endif

export module UnitTests_Timer;

import jpt.Timer;
import jpt.Timing.TypeDefs;
import jpt.TypeDefs;
import jpt.Utilities;

bool UnitTest_Timer()
{
    jpt::Timer timer;

    timer.Start();

    Sleep(1000);

    JPT_LOG(timer.GetElapsed());

    return true;
}

export bool RunUnitTests_Timer()
{
    JPT_ENSURE(UnitTest_Timer());

    return true;
}
