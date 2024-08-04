// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <mutex>

export module jpt.Mutex;

export namespace jpt
{
    class Mutex 
    {
        friend class ConditionVariable;

    private:
        std::mutex m_mutex;

    public:
        Mutex() = default;
        ~Mutex() = default;

        void Lock();
        void Unlock();
        bool TryLock();

        std::unique_lock<std::mutex> CreateUniqueLock();
    };

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