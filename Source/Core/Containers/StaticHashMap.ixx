// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <initializer_list>

export module jpt.StaticHashMap;

import jpt.Concepts;
import jpt.Constants;
import jpt.Comparators;
import jpt.Hash;
import jpt.Pair;
import jpt.StaticArray;
import jpt.TypeDefs;
import jpt.Utilities;

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
        using TStorage      = StaticArray<jpt_private::Entry<TKey, TValue>, kCapacity>;
        using TData         = jpt_private::Entry<TKey, TValue>::TData;
        using Iterator      = jpt_private::IteratorHashTable_LinearProbing<TKey, TValue, kCapacity>;
        using ConstIterator = jpt_private::ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity>;

    private:
        static constexpr TComparator kComparator = TComparator();

    private:
        TStorage m_array;
        Index m_count = 0;  /**< Count of actual elements in the map */

    public:
        constexpr StaticHashMap() = default;
        constexpr StaticHashMap(const std::initializer_list<TData>& list);
        constexpr StaticHashMap(const StaticHashMap& other);
        constexpr StaticHashMap(StaticHashMap&& other) noexcept;
        constexpr StaticHashMap& operator=(const StaticHashMap& other);
        constexpr StaticHashMap& operator=(StaticHashMap&& other) noexcept;
        constexpr ~StaticHashMap();

    public:
        // Adding
        constexpr TValue& Add(const TKey& key, const TValue& value);
        template<typename ...TArgs> constexpr TValue& Emplace(const TKey& key, TArgs&&... args);

        // Erasing
        constexpr Iterator Erase(const TKey& key) noexcept;
        constexpr Iterator Erase(const Iterator& iterator) noexcept;
        constexpr void Clear();

        // Accessing
        /** If key exists, return reference to it's associated value, caller may update it outside
            If key doesn't exist, Add a default value, return the inserted value too */
        constexpr TValue& operator[](const TKey& key);

        /** If key exists, return reference to it's associated value, caller can't update it outside
            If key doesn't exist, assertion fails */
        constexpr const TValue& operator[](const TKey& key) const;

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
        constexpr Iterator      Find(const TKey& key) noexcept;
        constexpr ConstIterator Find(const TKey& key) const noexcept;
        constexpr bool Has(const TKey& key) const noexcept;

    private:
        template<Iterable TContainer>
        constexpr void CopyData(const TContainer& container);
        constexpr void MoveMap(StaticHashMap&& other) noexcept;

        /** linearly probing starting from the hashed index of the key
            @return     An Index where
                            1. The slot with the same key provided
                            2. kInvalidIndex if the key is not found. O(n) operation */
        constexpr Index FindEqualIndex(const TKey& key) const noexcept;

        /** linearly probing starting from the hashed index of the key
            @return     An Index where
                            1. Empty slot
                            2. The slot with the same key provided
                            3. kInvalidIndex if the map is full and the key is not found. O(n) operation */
        constexpr Index FindEqualOrEmptyIndex(const TKey& key) const noexcept;
    };

    //----------------------------------------------------------------------------------------------
    // Non-member functions
    //----------------------------------------------------------------------------------------------
    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr bool operator==(const StaticHashMap<TKey, TValue, kCapacity, TComparator>& lhs, const StaticHashMap<TKey, TValue, kCapacity, TComparator>& rhs) noexcept
    {
        if (lhs.Count() != rhs.Count())
        {
            return false;
        }

        for (const auto& [key, value] : lhs)
        {
            if (rhs.Find(key) == rhs.end() || rhs.Find(key)->second != value)
            {
                return false;
            }
        }

        return true;
    }

    //----------------------------------------------------------------------------------------------
    // Member function definitions
    //----------------------------------------------------------------------------------------------
    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::StaticHashMap(const std::initializer_list<TData>& list)
    {
        CopyData(list);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::StaticHashMap(const StaticHashMap& other)
    {
        CopyData(other);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::StaticHashMap(StaticHashMap&& other) noexcept
    {
        MoveMap(Move(other));
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr  StaticHashMap<TKey, TValue, kCapacity, TComparator>& StaticHashMap<TKey, TValue, kCapacity, TComparator>::operator=(const StaticHashMap& other)
    {
        if (this != &other)
        {
            Clear();
            CopyData(other);
        }

        return *this;
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr  StaticHashMap<TKey, TValue, kCapacity, TComparator>& StaticHashMap<TKey, TValue, kCapacity, TComparator>::operator=(StaticHashMap&& other) noexcept
    {
        if (this != &other)
        {
            Clear();
            MoveMap(Move(other));
        }

        return *this;
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::~StaticHashMap()
    {
        Clear();
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr TValue& StaticHashMap<TKey, TValue, kCapacity, TComparator>::Add(const TKey& key, const TValue& value)
    {
        JPT_ASSERT(m_count < kCapacity, "StaticHashMap is full");

        const Index index = FindEqualOrEmptyIndex(key);
        JPT_ASSERT(index != kInvalidIndex);

        m_array[index].data = TData(key, value);
        m_array[index].isOccupied = true;
        ++m_count;

        return m_array[index].data.second;
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    template<typename ...TArgs>
    constexpr TValue& StaticHashMap<TKey, TValue, kCapacity, TComparator>::Emplace(const TKey& key, TArgs&&... args)
    {
        JPT_ASSERT(m_count < kCapacity, "StaticHashMap is full");

        const Index index = FindEqualOrEmptyIndex(key);
        JPT_ASSERT(index != kInvalidIndex);

        m_array[index].data = TData{ key, TValue(Forward<TArgs>(args)...) };
        m_array[index].isOccupied = true;
        ++m_count;

        return m_array[index].data.second;
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::Iterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::Erase(const TKey& key) noexcept
    {
        if (Iterator itr = Find(key); itr != end())
        {
            Iterator nextItr = itr + 1;

            const Index index = itr.GetIndex();
            m_array[index].isOccupied = false;
            --m_count;

            return nextItr;
        }

        return end();
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::Iterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::Erase(const Iterator& iterator) noexcept
    {
        return Erase(iterator->first);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr void StaticHashMap<TKey, TValue, kCapacity, TComparator>::Clear()
    {
        for (Index i = 0; i < kCapacity; ++i)
        {
            m_array[i].isOccupied = false;
        }

        m_count = 0;
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::TValue& StaticHashMap<TKey, TValue, kCapacity, TComparator>::operator[](const TKey& key)
    {
        Iterator itr = Find(key);
        if (itr == end())
        {
            return Emplace(key, {});
        }

        return itr->second;
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr const StaticHashMap<TKey, TValue, kCapacity, TComparator>::TValue& StaticHashMap<TKey, TValue, kCapacity, TComparator>::operator[](const TKey& key) const
    {
        ConstIterator itr = Find(key);
        JPT_ASSERT(itr != cend());
        return itr->second;
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::Iterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::begin() noexcept
    {
        if (IsEmpty())
        {
            return end();
        }

        return Iterator(m_array.Buffer(), 0);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::Iterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::end() noexcept
    {
        return Iterator(m_array.Buffer(), kCapacity);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::ConstIterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::begin() const noexcept
    {
        if (IsEmpty())
        {
            return end();
        }

        return ConstIterator(m_array.ConstBuffer(), 0);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::ConstIterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::end() const noexcept
    {
        return ConstIterator(m_array.ConstBuffer(), kCapacity);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::ConstIterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::cbegin() const noexcept
    {
        if (IsEmpty())
        {
            return cend();
        }

        return ConstIterator(m_array.ConstBuffer(), 0);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::ConstIterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::cend() const noexcept
    {
        return ConstIterator(m_array.ConstBuffer(), kCapacity);
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
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::Iterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::Find(const TKey& key) noexcept
    {
        const Index index = FindEqualIndex(key);
        if (index == kInvalidIndex)
        {
            return end();
        }

        return Iterator(m_array.Buffer(), index);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr StaticHashMap<TKey, TValue, kCapacity, TComparator>::ConstIterator StaticHashMap<TKey, TValue, kCapacity, TComparator>::Find(const TKey& key) const noexcept
    {
        const Index index = FindEqualIndex(key);
        if (index == kInvalidIndex)
        {
            return cend();
        }

        return ConstIterator(m_array.ConstBuffer(), index);
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr bool StaticHashMap<TKey, TValue, kCapacity, TComparator>::Has(const TKey& key) const noexcept
    {
        return Find(key) != cend();
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    template<Iterable TContainer>
    constexpr void StaticHashMap<TKey, TValue, kCapacity, TComparator>::CopyData(const TContainer& container)
    {
        for (const TData& data : container)
        {
            Emplace(data.first, data.second);
        }
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr void StaticHashMap<TKey, TValue, kCapacity, TComparator>::MoveMap(StaticHashMap&& other) noexcept
    {
        m_array = Move(other.m_array);
        m_count = other.m_count;

        other.m_count = 0;
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr Index StaticHashMap<TKey, TValue, kCapacity, TComparator>::FindEqualIndex(const TKey& key) const noexcept
    {
        if (IsEmpty())
        {
            return kInvalidIndex;
        }

        Index index = Hash(key) % kCapacity;
        const Index startIndex = index;
        while (!kComparator(m_array[index].data.first, key))
        {
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

        // If the slot is not occupied, means we erased it before, key shouldn't be found
        if (!m_array[index].isOccupied)
        {
            return kInvalidIndex;
        }

        return index;
    }

    template<typename TKey, typename TValue, Index kCapacity, typename TComparator>
    constexpr Index StaticHashMap<TKey, TValue, kCapacity, TComparator>::FindEqualOrEmptyIndex(const TKey& key) const noexcept
    {
        Index index = Hash(key) % kCapacity;

        // Linear probing to find the next available slot
        const Index startIndex = index;
        while (m_array[index].isOccupied)
        {
            // If the key is same, return the index
            if (kComparator(m_array[index].data.first, key))
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