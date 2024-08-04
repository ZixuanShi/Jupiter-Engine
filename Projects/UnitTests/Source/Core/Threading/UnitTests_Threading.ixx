// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

#include <type_traits>
#include <thread>

export module UnitTests_Threading;

import jpt.Thread;
import jpt.Thread.Utils;
import jpt.CPUInfo;

import jpt.TypeDefs;
import jpt.String;
import jpt.ToString;
import jpt.Utilities;
import jpt.DynamicArray;

static bool RawThreads()
{
    class TestThread final : public jpt::Thread_Base
    {
    public:
        TestThread(): 
            jpt::Thread_Base("Test Thread")
        {
        }

    protected:
        void Init() override 
        {
            JPT_LOG("Initializing thread " + m_name + jpt::ToString(GetId()));
        }

        void Update() override 
        {
            JPT_LOG("Updating thread " + m_name + jpt::ToString(GetId()));
            jpt::SleepForSeconds(1);
        }

        void Terminate() override 
        {
            JPT_LOG("Terminating thread " + m_name + jpt::ToString(GetId()));
        }
    };

    jpt::DynamicArray<TestThread> threads;
    threads.Reserve(jpt::GetNumCores());

    for (uint32 i = 0; i < jpt::GetNumCores(); ++i)
	{
		threads.EmplaceBack();
		threads.Back().Start();
	}

    jpt::SleepForSeconds(5);

    for (uint32 i = 0; i < jpt::GetNumCores(); ++i)
    {
        threads[i].Stop();
    }

    return true;
}

export bool RunUnitTests_Threading()
{
    JPT_ENSURE(RawThreads());

    return true;
}
