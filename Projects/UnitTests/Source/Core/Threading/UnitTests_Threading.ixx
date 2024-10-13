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

import jpt.CPUInfo;

import jpt.TypeDefs;
import jpt.String;
import jpt.ToString;
import jpt.Utilities;
import jpt.DynamicArray;

jpt::Mutex rawThreadsMutex;

static bool RawThreads()
{
    class TestThread final : public jpt::Thread_Base
    {
    public:
        TestThread(): 
            jpt::Thread_Base("Test Thread ")
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
            jpt::LockGuard lock(rawThreadsMutex);
            JPT_LOG("Terminating thread " + m_name);
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
                JPT_LOG("Produced: " + jpt::ToString(i));
                jpt::SleepMs(100);
            }

            m_shouldShutdown = true;
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
