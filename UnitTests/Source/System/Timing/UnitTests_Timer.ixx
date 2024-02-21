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
import jpt.Math;

bool UnitTest_Timer()
{
    jpt::Timer timer;

    timer.Start();

    Sleep(1000);

    JPT_ENSURE(jpt::AreValuesClose(timer.GetElapsed(), 1.0, 0.1));
    JPT_ENSURE(jpt::AreValuesClose(timer.GetElapsedMs(), 1000.0, 10.0));

    return true;
}

export bool RunUnitTests_Timer()
{
    JPT_ENSURE(UnitTest_Timer());

    return true;
}
