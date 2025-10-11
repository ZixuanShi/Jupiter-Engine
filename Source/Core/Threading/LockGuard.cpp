// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.LockGuard;

namespace jpt
{
    LockGuard::LockGuard(Mutex& mutex)
        : m_mutex(mutex)
    {
        m_mutex.Lock();
    }

    LockGuard::~LockGuard()
    {
        m_mutex.Unlock();
    }
}