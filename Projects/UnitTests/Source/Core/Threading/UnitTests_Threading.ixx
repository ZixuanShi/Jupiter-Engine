// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

#include <type_traits>
#include <thread>

export module UnitTests_Threading;

import jpt.Thread;
import jpt.Thread.Utils;
import jpt.ThreadSafeQueue;
import jpt.Mutex;
import jpt.LockGuard;

import jpt.Hardware.Manager;

import jpt.TypeDefs;
import jpt.String;
import jpt.ToString;
import jpt.Utilities;
import jpt.DynamicArray;

jpt::Mutex rawThreadsMutex;
jpt::Mutex rawThreadsShutdownMutex;

static bool RawThreads()
{
    class TestThread final : public jpt::Thread
    {
    public:
        TestThread(): 
            jpt::Thread("Test Thread ")
        {
        }

    protected:
        void Init() override 
        {
			jpt::LockGuard lock(rawThreadsMutex);

            static size_t s_id = 0;
			m_name += jpt::ToString(s_id);
			++s_id;

            JPT_LOG("Initializing thread " + m_name);
        }

        void Shutdown() override 
        {
            jpt::LockGuard lock(rawThreadsShutdownMutex);

            JPT_LOG("Terminating thread " + m_name);
        }
    };

	const uint32 numLogicalProcessors = jpt::GetLogicalProcessorsCount();
    jpt::DynamicArray<TestThread> threads;
    threads.Reserve(numLogicalProcessors);

    for (uint32 i = 0; i < numLogicalProcessors; ++i)
	{
		threads.EmplaceBack();
		threads.Back().Start();
	}

    jpt::Sleep(2);

    for (uint32 i = 0; i < numLogicalProcessors; ++i)
    {
        threads[i].Stop();
    }

    return true;
}

static bool ThreadSafeQueue()
{
    class ProducerThread final : public jpt::Thread
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
                JPT_LOG("Produced: " + jpt::ToString(i));
                jpt::SleepMs(100);
            }

            m_shouldShutdown = true;
        }
    };

    class ConsumerThread final : public jpt::Thread
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
            auto value = m_queue.TryPop();
            if (value)
            {
                JPT_LOG("Consumed: " + jpt::ToString(value.Value()));
            }
            else
            {
                jpt::SleepMs(10);
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

    producerThread.Stop();
    consumerThread.Stop();

    // Add a small delay to ensure threads have fully stopped
    jpt::SleepMs(100);

    return true;
}

export bool RunUnitTests_Threading()
{
    JPT_ENSURE(RawThreads());
    JPT_ENSURE(ThreadSafeQueue());

    return true;
}
