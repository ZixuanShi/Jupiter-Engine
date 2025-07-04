// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <shared_mutex>

export module jpt.SharedMutex;

export namespace jpt
{
    using UniqueLock = std::unique_lock<std::shared_mutex>;
    using SharedLock = std::shared_lock<std::shared_mutex>;

    /** Protects a scope can be accessed by multiple readers or one writer at the same time */
    class SharedMutex
    {
    private:
        std::shared_mutex m_mutex;

    public:
        /** Only one thread can write */
        [[nodiscard]] UniqueLock CreateUniqueLock();

        /** Multiple threads can read */
        [[nodiscard]] SharedLock CreateSharedLock();
    };

    [[nodiscard]] UniqueLock SharedMutex::CreateUniqueLock()
    {
        return UniqueLock(m_mutex);
    }

    [[nodiscard]] SharedLock SharedMutex::CreateSharedLock()
    {
        return SharedLock(m_mutex);
    }
}