// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

export module jpt.StaticHashMap;

import jpt.Constants;
import jpt.Comparators;
import jpt.Hash;
import jpt.Pair;
import jpt.StaticArray;
import jpt.TypeDefs;

import jpt_private.IteratorHashTable_LinearProbing;

export namespace jpt
{
    /** A fixed-size hash map implementation using a static array for storage, and linear probing for hash-collision.
        Not using heap allocation at all. */
    template<typename _TKey, typename _TValue, Index kCapacity, typename _TComparator = Comparator_Equal<_TKey>>
    class StaticHashMap
    {
    public:
        using TKey          = _TKey;
        using TValue        = _TValue;
        using TComparator   = _TComparator;
        using TData         = Pair<TKey, TValue>;
        using TBuckets      = StaticArray<TData, kCapacity>;
        using Iterator      = jpt_private::IteratorHashTable_LinearProbing<TData>;
        using ConstIterator = jpt_private::ConstIteratorHashTable_LinearProbing<TData>;

    private:
        static constexpr TComparator kComparator = TComparator();

    private:
        TBuckets m_buckets;
        size_t m_count = 0;  /**< Count of actual elements in the map */

    public:
        constexpr StaticHashMap() = default;

    public:
        // Adding
        constexpr TValue& Add(const TKey& key, const TValue& value);

        // Accessing
        constexpr TValue& operator[](const TKey& key) noexcept;
        
        // Capacity
        constexpr size_t Count() const noexcept;
        constexpr bool IsEmpty() const noexcept;

        // Searching
        constexpr Iterator      Find(const TKey& key) noexcept;
        //constexpr ConstIterator Find(const TKey& key) const noexcept;

    private:
        constexpr Index GetBucketIndex(const TKey& key) const noexcept;
    };

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr TValue& StaticHashMap<TKey, TValue, kCapacity, TComparator>::Add(const TKey& key, const TValue& value)
    {
        JPT_ASSERT(m_count < kCapacity, "StaticHashMap is full, cannot add more elements.");
        

    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr TValue& StaticHashMap<TKey, TValue, kCapacity, TComparator>::operator[](const TKey& key) noexcept
    {
        
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr size_t StaticHashMap<TKey, TValue, kCapacity, TComparator>::Count() const noexcept
    {
        return m_count;
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr bool StaticHashMap<TKey, TValue, kCapacity, TComparator>::IsEmpty() const noexcept
    {
        return m_count == 0;
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr Index StaticHashMap<TKey, TValue, kCapacity, TComparator>::GetBucketIndex(const TKey& key) const noexcept
    {
        return Hash(key) % kCapacity;
    }
}