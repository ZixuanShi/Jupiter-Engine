// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

export module jpt.UniqueString;

import jpt.Hash;
import jpt.HashMap;
import jpt.String;
import jpt.TypeDefs;

export namespace jpt
{
    /** Unique string through out application. Unreal's FName equivalent
        @example:
            UniqueString uniqueStr("Hello");    // Allocates a new string
            UniqueString uniqueStr2("Hello");   // Reuses the same string */
    class UniqueString
    {
    private:
        // Todo. Replace with ThreadSafeHashMap when available
        static inline HashMap<uint64, String> s_uniqueStrings;  /**< Hash map to store unique strings */

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
        if (!s_uniqueStrings.Has(m_hash))
        {
            s_uniqueStrings.Emplace(m_hash, str);
        }
    }

    const String& UniqueString::operator*() const
    {
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
        s_uniqueStrings.Clear();
    }

    void UniqueString::Erase(const char* str)
    {
        s_uniqueStrings.Erase(Hash(str));
    }
}