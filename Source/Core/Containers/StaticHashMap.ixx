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
        using TBuckets      = StaticArray<jpt_private::Entry<TKey, TValue>, kCapacity>;
        using TData         = jpt_private::Entry<TKey, TValue>::TData;
        using Iterator      = jpt_private::IteratorHashTable_LinearProbing<TKey, TValue, kCapacity>;
        using ConstIterator = jpt_private::ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity>;

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
        constexpr Iterator begin() noexcept;
        constexpr Iterator end()   noexcept;
        constexpr ConstIterator begin()  const noexcept;
        constexpr ConstIterator end()    const noexcept;
        constexpr ConstIterator cbegin() const noexcept;
        constexpr ConstIterator cend()   const noexcept;
        
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
        JPT_ASSERT(m_count < kCapacity, "StaticHashMap is full");

        const Index index = GetBucketIndex(key);
        JPT_ASSERT(index != kInvalidIndex);

        m_buckets[index].data = TData(key, value);
        m_buckets[index].isOccupied = true;
        ++m_count;

        return m_buckets[index].data.second;
    }

    //template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    //constexpr TValue& StaticHashMap<TKey, TValue, kCapacity, TComparator>::operator[](const TKey& key) noexcept
    //{

    //}

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::Iterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::begin() noexcept
    {
        if (IsEmpty())
        {
            return end();
        }

        return Iterator(m_buckets.Buffer(), 0);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::Iterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::end() noexcept
    {
        return Iterator(m_buckets.Buffer(), kCapacity);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::ConstIterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::begin() const noexcept
    {
        if (IsEmpty())
        {
            return end();
        }

        return Iterator(m_buckets.Buffer(), 0);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::ConstIterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::end() const noexcept
    {
        return Iterator(m_buckets.Buffer(), kCapacity);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::ConstIterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::cbegin() const noexcept
    {
        if (IsEmpty())
        {
            return end();
        }

        return ConstIterator(m_buckets.Buffer(), 0);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::ConstIterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::cend() const noexcept
    {
        return ConstIterator(m_buckets.Buffer(), kCapacity);
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

    //template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    //constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::Iterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::Find(const TKey& key) noexcept
    //{
    //    
    //}

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr Index StaticHashMap<TKey, TValue, kCapacity, TComparator>::GetBucketIndex(const TKey& key) const noexcept
    {
        Index index = Hash(key) % kCapacity;

        // Linear probing to find the next available bucket
        const Index startIndex = index;
        while (m_buckets[index].isOccupied)
        {
            // If the key is same, return the index
            if (kComparator(m_buckets[index].data.first, key))
            {
                break;
            }

            ++index;

            // Wrap around to the start
            if (index >= kCapacity)
            {
                index = 0;  
            }

            // If we circled back to the start index, it means the map is full or the key is not found
            if (index == startIndex)
            {
                return kInvalidIndex;            
            }
        }

        return index;
    }
}