// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <mutex>

export module jpt.Mutex;

export namespace jpt
{
    /** Protects a scope can be only accessed by up-to-one thread at the same time */
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
}