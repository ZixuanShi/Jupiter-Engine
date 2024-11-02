// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

#include <thread>
#include <atomic>

export module jpt.Thread;

import jpt.Atomic;
import jpt.Hash;
import jpt.String;
import jpt.TypeDefs;
import jpt.UniquePtr;
import jpt.Utilities;

export namespace jpt
{
	/** Base class for all threads
		@example:
		    class TestThread final : public jpt::Thread
            {
            protected:
                void Init() override 
                {
                    JPT_LOG("Initializing thread " + m_name + jpt::ToString(GetId()));
                }

                void Update() override 
                {
                    JPT_LOG("Updating thread " + m_name + jpt::ToString(GetId()));
                    jpt::Sleep(1);
                }

                void Shutdown() override 
                {
                    JPT_LOG("Terminating thread " + m_name + jpt::ToString(GetId()));
                }
            }; */
    class Thread
    {
    protected:
        String m_name = "Unnamed";
        Atomic<bool> m_isRunning{ false };
        Atomic<bool> m_shouldShutdown{ false };

    private:
        UniquePtr<std::thread> m_thread;

    public:
        Thread() noexcept = default;
        Thread(const char* name) noexcept;
        virtual ~Thread() noexcept;

        Thread(Thread&& other) noexcept;
        Thread& operator=(Thread&& other) noexcept;

        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

        void Start();
        void Stop();

        const String& GetName() const noexcept;

    protected:
        virtual void Init() {}
        virtual void Update() {}
        virtual void Shutdown() {}
    };

    Thread::Thread(const char* name) noexcept
        : m_name(name) 
    {
    }

    Thread::~Thread() noexcept
    {
        Stop();
    }

    Thread::Thread(Thread&& other) noexcept
        : m_name(Move(other.m_name))
        , m_isRunning(other.m_isRunning.Load())
        , m_shouldShutdown(other.m_shouldShutdown.Load())
        , m_thread(Move(other.m_thread))
    {
        other.m_isRunning = false;
        other.m_shouldShutdown = true;
    }

    Thread& Thread::operator=(Thread&& other) noexcept
    {
        if (this != &other)
        {
            Stop();

            m_name = Move(other.m_name);
            m_isRunning = other.m_isRunning.Load();
            m_shouldShutdown = other.m_shouldShutdown.Load();
            m_thread = Move(other.m_thread);

            other.m_isRunning = false;
            other.m_shouldShutdown = true;
        }

        return *this;
    }

    void Thread::Start()
    {
        if (!m_isRunning)
        {
            m_shouldShutdown = false;
            m_isRunning = true;
            m_thread = MakeUnique<std::thread>([this]()
                {
                    Init();
                    while (!m_shouldShutdown)
                    {
                        Update();
                    }
                    Shutdown();
                    m_isRunning = false;
                });
        }
    }

    void Thread::Stop()
    {
        m_shouldShutdown = true;
        if (m_thread && m_thread->joinable())
        {
            m_thread->join();
        }
        m_isRunning = false;
    }

    const String& Thread::GetName() const noexcept
    {
        return m_name;
    }
}