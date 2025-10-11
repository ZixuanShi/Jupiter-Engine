// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

module jpt.UniqueString;

namespace jpt
{
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