// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"
#include "Profiling/TimingProfiler.h"

#include <type_traits>
#include <thread>
#include <atomic>
#include <mutex>

export module UnitTests_Threading;

import jpt.Thread;
import jpt.ThreadUtils;
import jpt.ThreadSafeQueue;
import jpt.Mutex;
import jpt.LockGuard;

import jpt.Event;
import jpt.EventManager;

import jpt.HardwareManager;

import jpt.TypeDefs;
import jpt.String;
import jpt.ToString;
import jpt.Utilities;
import jpt.StaticArray;
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

            m_isActive = false;
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

class TestThread final : public jpt::Thread
{
public:
    TestThread() :
        jpt::Thread("Test Thread Hello")
    {
    }

protected:
    virtual void Update() override
    {
        JPT_LOG("Hello");
        jpt::Sleep(1);
    }
};
class TestThread2 final : public jpt::Thread
{
public:
    TestThread2() :
        jpt::Thread("Test Thread Bye")
    {
    }

protected:
    virtual void Update() override
    {
        JPT_LOG("Bye");
        jpt::Sleep(2);
    }
};

TestThread thread;
TestThread2 thread2;

class Event_ShutdownThread final : public jpt::Event
{
private:
    jpt::Thread* m_pThread;

public:
    Event_ShutdownThread(jpt::Thread* pThread) :
        m_pThread(pThread)
    {
    }

    jpt::Thread* GetThread() const { return m_pThread; }
};

static bool NotBlockingMain()
{
    thread.Start();
    thread2.Start();

    Event_ShutdownThread eventShutdownThread(&thread);
    jpt::EventManager::GetInstance().Queue(eventShutdownThread, 5.0f);

    Event_ShutdownThread eventShutdownThread2(&thread2);
    jpt::EventManager::GetInstance().Queue(eventShutdownThread2, 10.0f);

    jpt::EventManager::GetInstance().Register<Event_ShutdownThread>([](const Event_ShutdownThread& event)
        {
            event.GetThread()->Stop();
            JPT_LOG("Stopped Thread: %s", event.GetThread()->GetName().ConstBuffer());
        });

    return true;
}


std::mutex g_mutex;
bool MutexVsAtomic(size_t threadsCount)
{
    jpt::DynamicArray<uint32> data(1024, 1);

    jpt::Atomic<uint32> sum = 0;

    {
        JPT_SCOPED_TIMING_PROFILER("MutexVsAtomic");

        jpt::DynamicArray<std::thread> threads;
        threads.Reserve(threadsCount);

        const size_t chunkSize = data.Count() / threadsCount;

        for (size_t i = 0; i < threadsCount; ++i)
        {
            const size_t startIndex = i * chunkSize;
            const size_t endIndex = startIndex + chunkSize;

            threads.EmplaceBack([startIndex, endIndex, &data, &sum]() 
                {
                    //std::lock_guard<std::mutex> lock(g_mutex); // Mutex is slow because it locks the entire data structure. Use atomic for counter or flag

                    for (size_t j = startIndex; j < endIndex; ++j)
                    {
                        sum += data[j];
                    }
                });
        }

        for (size_t i = 0; i < threadsCount; ++i)
        {
            threads[i].join();
        }
    }

    return sum == data.Count();
}

export bool RunUnitTests_Threading()
{
    JPT_ENSURE(MutexVsAtomic(2));
    JPT_ENSURE(MutexVsAtomic(4));
    JPT_ENSURE(MutexVsAtomic(16));

    JPT_ENSURE(RawThreads());
    JPT_ENSURE(ThreadSafeQueue());
    JPT_ENSURE(NotBlockingMain());

    return true;
}
