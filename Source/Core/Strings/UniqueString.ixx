// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

export module jpt.UniqueString;

import jpt.Hash;
import jpt.HashMap;
import jpt.String;
import jpt.TypeDefs;
import jpt.ThreadUtils;

export namespace jpt
{
    /** Unique string through out application. Unreal's FName equivalent, but only on the main thread for optimal performance.
        @example:
            UniqueString uniqueStr("Hello");    // Allocates a new string
            UniqueString uniqueStr2("Hello");   // Reuses the same string */
    class UniqueString
    {
    private:
        static inline HashMap<uint64, String> s_uniqueStrings;  /**< Shared Hash map to store unique strings */
        static inline Index s_mainThreadId = GetThreadId();       /**< Main thread ID, used to check if the current thread is the main thread */

        uint64 m_hash = 0;    /**< Unique Identifier, key in the map to retreive the unique string */

    public:
        UniqueString(const char* str);

        const String& operator*() const;
        uint64 GetHash() const;

    public:
        static void Clear();
        static void Erase(const char* str);
    };

    // ------------------------------------------------------------------------------------------------
    // Non-Member Functions
    // ------------------------------------------------------------------------------------------------
    bool operator==(const UniqueString& lhs, const UniqueString& rhs)
    {
        return lhs.GetHash() == rhs.GetHash();
    }

    String ToString(const UniqueString& uniqueStr)
    {
        return *uniqueStr;
    }
    
    // ------------------------------------------------------------------------------------------------
    // Member Functions
    // ------------------------------------------------------------------------------------------------
    UniqueString::UniqueString(const char* str)
        : m_hash(Hash(str))
    {
        JPT_ASSERT(s_mainThreadId == GetThreadId(), "UniqueString can only be created on the main thread");

        if (!s_uniqueStrings.Has(m_hash))
        {
            s_uniqueStrings.Emplace(m_hash, str);
        }
    }

    const String& UniqueString::operator*() const
    {
        JPT_ASSERT(s_mainThreadId == GetThreadId(), "UniqueString can only be read from the main thread");
        JPT_ASSERT(s_uniqueStrings.Has(m_hash), "UniqueString not found in the map");

        return s_uniqueStrings[m_hash];
    }

    uint64 UniqueString::GetHash() const
    {
        return m_hash;
    }

    // ------------------------------------------------------------------------------------------------
    // Static Member Functions
    // ------------------------------------------------------------------------------------------------
    void UniqueString::Clear()
    {
        JPT_ASSERT(s_mainThreadId == GetThreadId(), "UniqueString can only be cleared on the main thread");

        s_uniqueStrings.Clear();
    }

    void UniqueString::Erase(const char* str)
    {
        JPT_ASSERT(s_mainThreadId == GetThreadId(), "UniqueString can only be erased on the main thread");

        s_uniqueStrings.Erase(Hash(str));
    }
}