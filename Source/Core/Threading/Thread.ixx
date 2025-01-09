// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

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
        Atomic<bool> m_isActive{ false };

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

        if (m_thread && m_thread->joinable())
        {
            m_thread->join();
        }
    }

    Thread::Thread(Thread&& other) noexcept
        : m_name(Move(other.m_name))
        , m_isActive(other.m_isActive.Load())
        , m_thread(Move(other.m_thread))
    {
        other.m_isActive = false;
    }

    Thread& Thread::operator=(Thread&& other) noexcept
    {
        if (this != &other)
        {
            Stop();

            m_name = Move(other.m_name);
            m_isActive = other.m_isActive.Load();
            m_thread = Move(other.m_thread);

            other.m_isActive = false;
            other.m_isActive = true;
        }

        return *this;
    }

    void Thread::Start()
    {
        if (!m_isActive)
        {
            m_isActive = true;
            m_thread = MakeUnique<std::thread>([this]()
                {
                    Init();

                    while (m_isActive)
                    {
                        Update();
                    }

                    Shutdown();
                    Stop();
                });
        }
    }

    void Thread::Stop()
    {
        m_isActive = false;
    }

    const String& Thread::GetName() const noexcept
    {
        return m_name;
    }
}