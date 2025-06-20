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

    private:
        struct Entry
        {
            TData data;
            bool isOccupied = false;
        };

    public:
        using TBuckets      = StaticArray<Entry, kCapacity>;
        using Iterator      = TData*;
        using ConstIterator = const TData*;

    private:
        static constexpr TComparator kComparator = TComparator();

    private:
        TBuckets m_buckets;
        Index m_count = 0;  /**< Count of actual elements in the map */

    public:
        constexpr StaticHashMap() = default;

    public:
        // Adding
        constexpr TValue& Add(const TKey& key, const TValue& value);

        // Accessing
        //constexpr TValue& operator[](const TKey& key) noexcept;

        // Iterators    
 /*       constexpr Iterator begin() noexcept;
        constexpr Iterator end()   noexcept;
        constexpr ConstIterator begin()  const noexcept;
        constexpr ConstIterator end()    const noexcept;
        constexpr ConstIterator cbegin() const noexcept;
        constexpr ConstIterator cend()   const noexcept;*/
        
        // Capacity
        constexpr size_t Count() const noexcept;
        constexpr bool IsEmpty() const noexcept;

        // Searching
        //constexpr Iterator      Find(const TKey& key) noexcept;
        //constexpr ConstIterator Find(const TKey& key) const noexcept;

    private:
        constexpr Index GetBucketIndex(const TKey& key) const noexcept;
    };

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr TValue& StaticHashMap<TKey, TValue, kCapacity, TComparator>::Add(const TKey& key, const TValue& value)
    {
        if (m_count == kCapacity)
        {
            JPT_ASSERT(false, "StaticHashMap is full, cannot add more elements.");
            return m_buckets[0].data.second; // Return a reference to the first element's value (undefined behavior)
        }
        
        Index index = GetBucketIndex(key);
        while (m_buckets[index].isOccupied)
        {
            ++index;
        }

        m_buckets[index].data = TData(key, value);
        m_buckets[index].isOccupied = true;

        ++m_count;

        return m_buckets[index].data.second; // Return a reference to the value
    }

    //template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    //constexpr TValue& StaticHashMap<TKey, TValue, kCapacity, TComparator>::operator[](const TKey& key) noexcept
    //{
    //    
    //}

    //template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    //constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::Iterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::begin() noexcept
    //{
    //    return m_buckets.begin();
    //}

    //template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    //constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::Iterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::end() noexcept
    //{
    //    return m_buckets.end();
    //}

    //template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    //constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::ConstIterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::begin() const noexcept
    //{
    //    return m_buckets.begin();
    //}

    //template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    //constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::ConstIterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::end() const noexcept
    //{
    //    return m_buckets.end();
    //}

    //template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    //constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::ConstIterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::cbegin() const noexcept
    //{
    //    return m_buckets.cbegin();
    //}

    //template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    //constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::ConstIterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::cend() const noexcept
    //{
    //    return m_buckets.cend();
    //}

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

    //template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    //constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::Iterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::Find(const TKey& key) noexcept
    //{
    //    
    //}

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr Index StaticHashMap<TKey, TValue, kCapacity, TComparator>::GetBucketIndex(const TKey& key) const noexcept
    {
        return Hash(key) % kCapacity;
    }
}