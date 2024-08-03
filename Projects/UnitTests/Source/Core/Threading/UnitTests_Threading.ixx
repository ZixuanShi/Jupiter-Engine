// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Threading;

import jpt.Thread;
import jpt.Thread.Utils;
import jpt.TypeDefs;
import jpt.Utilities;

static bool RawThreads()
{
    class TestThread final : public jpt::Thread_Base
    {
    protected:
        void Init() override 
        {
            JPT_LOG("Initializing thread");
        }

        void Update() override 
        {
            JPT_LOG("Updating thread");
            jpt::SleepForSeconds(1);
        }

        void Terminate() override 
        {
            JPT_LOG("Terminating thread");
        }
    };

    TestThread myThread;
    myThread.Start();

    // Let the thread run for 5 seconds
    jpt::SleepForSeconds(5);

    myThread.Stop();

    return true;
}

export bool RunUnitTests_Threading()
{
    JPT_ENSURE(RawThreads());

    return true;
}
