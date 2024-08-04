// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

#include <type_traits>
#include <thread>

export module UnitTests_Threading;

import jpt.Thread;
import jpt.Thread.Utils;
import jpt.ThreadSafeQueue;

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

    jpt::Sleep(2);

    for (uint32 i = 0; i < jpt::GetNumCores(); ++i)
    {
        threads[i].Stop();
    }

    return true;
}

static bool ThreadSafeQueue()
{
    class ProducerThread final : public jpt::Thread_Base
    {
    private:
        jpt::ThreadSafeQueue<int32>& m_queue;

    public:
        ProducerThread(jpt::ThreadSafeQueue<int32>& queue)
            : m_queue(queue)
        {
        }

    protected:
        void Init() override 
		{
            for (int32 i = 0; i < 10; ++i)
			{
				m_queue.Push(i);
                jpt::SleepMs(100);
                JPT_LOG("Produced: " + jpt::ToString(i));
			}
		}
    };

    class ConsumerThread final : public jpt::Thread_Base
    {
    private:
        jpt::ThreadSafeQueue<int32>& m_queue;

    public:
        ConsumerThread(jpt::ThreadSafeQueue<int32>& queue)
            : m_queue(queue)
        {
        }

    protected:
        void Update() override
        {
            auto value = m_queue.WaitPop();
            if (value) 
            {
                JPT_LOG("Consumed: " + jpt::ToString(value.Value()));
            }
            else 
            {
                // Queue is shut down and empty
                m_isRunning = false;
            }
        }
    };
    
    jpt::ThreadSafeQueue<int32> queue;

    ProducerThread producerThread(queue);
    ConsumerThread consumerThread(queue);

    producerThread.Start();
    consumerThread.Start();

    // Allow some time for operations
    jpt::SleepMs(2000);

    queue.Terminate(); // Signal the queue to shut down
    producerThread.Stop();
    consumerThread.Stop();

    return true;
}

export bool RunUnitTests_Threading()
{
    //JPT_ENSURE(RawThreads());
    JPT_ENSURE(ThreadSafeQueue());

    return true;
}
