// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <shared_mutex>

export module jpt.SharedMutex;

export namespace jpt
{
    /** Protects a scope can be accessed by multiple readers or one writer at the same time */
    class SharedMutex
    {
    private:
        std::shared_mutex m_mutex;

    public:
        SharedMutex() = default;
        ~SharedMutex() = default;

        void Lock();
        void Unlock();
        bool TryLock();

        std::unique_lock<std::shared_mutex> CreateUniqueLock();
        std::shared_lock<std::shared_mutex> CreateSharedLock();
    };

    void SharedMutex::Lock()
    {
        m_mutex.lock();
    }

    void SharedMutex::Unlock()
    {
        m_mutex.unlock();
    }

    bool SharedMutex::TryLock()
    {
        return m_mutex.try_lock();
    }

    std::unique_lock<std::shared_mutex> SharedMutex::CreateUniqueLock()
    {
        return std::unique_lock<std::shared_mutex>(m_mutex);
    }

    std::shared_lock<std::shared_mutex> SharedMutex::CreateSharedLock()
    {
        return std::shared_lock<std::shared_mutex>(m_mutex);
    }
}