// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.SharedMutex;

namespace jpt
{
    [[nodiscard]] UniqueLock SharedMutex::CreateUniqueLock()
    {
        return UniqueLock(m_mutex);
    }

    [[nodiscard]] SharedLock SharedMutex::CreateSharedLock()
    {
        return SharedLock(m_mutex);
    }
}