// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

export module jpt.StaticHashMap;

import jpt.Comparators;
import jpt.Hash;
import jpt.Pair;
import jpt.StaticArray;
import jpt.TypeDefs;

export namespace jpt
{
    /** A fixed-size hash map implementation using a static array for storage, and linear probing for hash-collision.
        Not using heap allocation at all. */
    template<typename _TKey, typename _TValue, Index kCount, typename _TComparator = Comparator_Equal<_TKey>>
    class StaticHashMap
    {
    public:
        using TKey        = _TKey;
        using TValue      = _TValue;
        using TComparator = _TComparator;
        using TData       = Pair<TKey, TValue>;
        using TBuckets    = StaticArray<TData, kCount>;

    private:
        static constexpr TComparator kComparator = TComparator();
        static constexpr size_t kInvalidIndex = kCount;  /**< Index that indicates an empty bucket */

    private:
        TBuckets m_buckets;
        size_t m_count = 0;  /**< Count of actual elements in the map */

    public:
        constexpr StaticHashMap() = default;

    public:
        // Accessing
        constexpr TValue& operator[](const TKey& key) noexcept;
        
        // Capacity
        constexpr size_t Count() const noexcept;
        constexpr bool IsEmpty() const noexcept;

        // Searching
        

    private:
        
    };

    template<typename TKey, typename TValue, Index kCount, typename TComparator>
    constexpr TValue& StaticHashMap<TKey, TValue, kCount, TComparator>::operator[](const TKey& key) noexcept
    {



        ++m_count;
    }

    template<typename TKey, typename TValue, Index kCount, typename TComparator>
    constexpr size_t StaticHashMap<TKey, TValue, kCount, TComparator>::Count() const noexcept
    {
        return m_count;
    }

    template<typename TKey, typename TValue, Index kCount, typename TComparator>
    constexpr bool StaticHashMap<TKey, TValue, kCount, TComparator>::IsEmpty() const noexcept
    {
        return m_count == 0;
    }
}