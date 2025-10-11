// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.Mutex;

namespace jpt
{
    void Mutex::Lock()
    {
        m_mutex.lock();
    }

    void Mutex::Unlock()
    {
        m_mutex.unlock();
    }

    bool Mutex::TryLock()
    {
        return m_mutex.try_lock();
    }

    std::unique_lock<std::mutex> Mutex::CreateUniqueLock()
    {
        return std::unique_lock<std::mutex>(m_mutex);
    }
}