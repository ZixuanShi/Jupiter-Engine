// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

module jpt.Thread;

import jpt.Utilities;

namespace jpt
{
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

                    Terminate();
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