// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

#include <thread>
#include <atomic>

export module jpt.Thread;

import jpt.Atomic;
import jpt.UniquePtr;
import jpt.String;
import jpt.TypeDefs;
import jpt.Utilities;

export namespace jpt
{
	/** Base class for all threads
		@example:
		    class TestThread final : public jpt::Thread_Base
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

                void Terminate() override 
                {
                    JPT_LOG("Terminating thread " + m_name + jpt::ToString(GetId()));
                }
            }; */
    class Thread_Base
    {
    protected:
        String m_name = "Unnamed";
        Atomic<bool> m_isRunning{ false };
        Atomic<bool> m_shouldTerminate{ false };

    private:
        UniquePtr<std::thread> m_thread;

    public:
        Thread_Base() noexcept = default;
        Thread_Base(const char* name) noexcept;
        virtual ~Thread_Base() noexcept;

        Thread_Base(Thread_Base&& other) noexcept;
        Thread_Base& operator=(Thread_Base&& other) noexcept;

        Thread_Base(const Thread_Base&) = delete;
        Thread_Base& operator=(const Thread_Base&) = delete;

        void Start();
        void Stop();

        const String& GetName() const noexcept;
        uint32 GetId() const noexcept;

    protected:
        virtual void Init() {}
        virtual void Update() {}
        virtual void Terminate() {}
    };

    Thread_Base::Thread_Base(const char* name) noexcept
        : m_name(name) 
    {
    }

    Thread_Base::~Thread_Base() noexcept
    {
        Stop();
    }

    Thread_Base::Thread_Base(Thread_Base&& other) noexcept
        : m_name(Move(other.m_name))
        , m_isRunning(other.m_isRunning.Load())
        , m_shouldTerminate(other.m_shouldTerminate.Load())
        , m_thread(Move(other.m_thread))
    {
        other.m_isRunning = false;
        other.m_shouldTerminate = true;
    }

    Thread_Base& Thread_Base::operator=(Thread_Base&& other) noexcept
    {
        if (this != &other)
        {
            Stop();

            m_name = Move(other.m_name);
            m_isRunning = other.m_isRunning.Load();
            m_shouldTerminate = other.m_shouldTerminate.Load();
            m_thread = Move(other.m_thread);

            other.m_isRunning = false;
            other.m_shouldTerminate = true;
        }

        return *this;
    }

    void Thread_Base::Start()
    {
        if (!m_isRunning)
        {
            m_shouldTerminate = false;
            m_isRunning = true;
            m_thread = MakeUnique<std::thread>([this]()
                {
                    Init();
                    while (!m_shouldTerminate)
                    {
                        Update();
                    }
                    Terminate();
                    m_isRunning = false;
                });
        }
    }

    void Thread_Base::Stop()
    {
        m_shouldTerminate = true;
        if (m_thread && m_thread->joinable())
        {
            m_thread->join();
        }
        m_isRunning = false;
    }

    const String& Thread_Base::GetName() const noexcept
    {
        return m_name;
    }

    uint32 Thread_Base::GetId() const noexcept
    {
        return m_thread ? m_thread->get_id()._Get_underlying_id() : 0;
    }
}