// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.UniqueString;

import jpt.Hash;
import jpt.HashMap;
import jpt.String;
import jpt.TypeDefs;

export namespace jpt
{
    /** Unique string through out application. Unreal's FName equivalent.
        @example:
            UniqueString uniqueStr("Hello");    // Heap-Allocate a string
            UniqueString uniqueStr2("Hello");   // Reuses the same string */
    class UniqueString
    {
    private:
        static inline HashMap<uint64, String> s_uniqueStrings;  /**< Shared Hash map to store unique strings */

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
}