// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Utilities.h"
#include "Core/Validation/Assert.h"

#include <initializer_list>
#include <compare>

export module jpt.HashMap;

import jpt.Comparators;
import jpt.Concepts;
import jpt.Constants;
import jpt.DynamicArray;
import jpt.Hash;
import jpt.LinkedList;
import jpt.Math;
import jpt.Pair;
import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Serializer;

import jpt_private.Iterator_HashTable_Chaining;

export namespace jpt
{
    /** Unordered map implementation with Chainning hash collision-handling  */
    template <typename _TKey, typename _TValue, typename _TComparator = Comparator_Equal<_TKey>>
    class HashMap
    {
    public:
        using TKey          = _TKey;
        using TValue        = _TValue;
        using TComparator   = _TComparator;
        using Data          = Pair<const TKey, TValue>;
        using Bucket        = LinkedList<Data>;
        using Buckets       = DynamicArray<Bucket>;
        using Iterator      = jpt_private::Iterator_HashTable_Chaining<Data>;
        using ConstIterator = jpt_private::ConstIterator_HashTable_Chaining<Data>;

    private:
        static constexpr TComparator kComparator = TComparator();
        static constexpr float kLoadFactor = 0.75f;
        static constexpr Index kGrowMultiplier = 2;

    private:
        Buckets m_buckets; 
        Index m_count = 0;          /**< Count of actual elements in the map */

    public:
        constexpr HashMap() = default;
        constexpr HashMap(const std::initializer_list<Data>& list);
        constexpr HashMap(const HashMap& other);
        constexpr HashMap(HashMap&& other) noexcept;
        constexpr HashMap& operator=(const HashMap& other);
        constexpr HashMap& operator=(HashMap&& other) noexcept;
        constexpr ~HashMap();
        
    public:
        // Adding
        constexpr TValue& Add(const TKey& key, const TValue& value);
        constexpr TValue& Add(const Data& element);
        constexpr TValue& Add(TKey&& key, TValue&& value);
        constexpr TValue& Add(Data&& element);
        template<typename ...TArgs> constexpr TValue& Emplace(const TKey& key, TArgs&&... args);

        // Erasing
        constexpr Iterator Erase(const TKey& key);
        constexpr Iterator Erase(const Iterator& iterator);
        constexpr void Clear();

        // Accessing
        /** If key exists, return reference to it's associated value, caller may update it outside
            If key doesn't exist, Add a default value, return the inserted value too */
        constexpr       TValue& operator[](const TKey& key);

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
        constexpr Index Count() const noexcept;
        constexpr bool IsEmpty() const noexcept;

        // Modifiers
        constexpr void ResizeBuckets(Index capacity);

        // Searching
        constexpr Iterator      Find(const TKey& key);
        constexpr ConstIterator Find(const TKey& key) const;
        constexpr bool Has(const TKey& key) const;

        // Serialization
        void Serialize(Serializer& serializer) const;
        void Deserialize(Serializer& serializer);

    private:
        constexpr Index GetBucketIndex(const TKey& key) const;
        constexpr Index GetBucketIndex(const TKey& key, Index bucketCount) const;
        constexpr       Bucket& GetBucket(const TKey& key);
        constexpr const Bucket& GetBucket(const TKey& key) const;

        template<Iterable TContainer>
        constexpr void CopyData(const TContainer& container, Index size);
        constexpr void MoveMap(HashMap&& other);
    };

    //----------------------------------------------------------------------------------------------
    // Non-member functions
    //----------------------------------------------------------------------------------------------
    template<typename TKey, typename TValue, typename TComparator>
    constexpr bool operator==(const HashMap<TKey, TValue, TComparator>& lhs, const HashMap<TKey, TValue, TComparator>& rhs)
    {
        if (lhs.Count() != rhs.Count())
        {
            return false;
        }

        for (const auto& [key, value] : lhs)
        {
            if (rhs.Has(key) == false || rhs[key] != value)
            {
                return false;
            }
        }

        return true;
    }

    //----------------------------------------------------------------------------------------------
    // Member function definitions
    //----------------------------------------------------------------------------------------------
    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::HashMap(const std::initializer_list<Data>& list)
    {
        CopyData(list, list.size());
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::HashMap(const HashMap& other)
    {
        CopyData(other, other.Count());
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::HashMap(HashMap&& other) noexcept
    {
        MoveMap(Move(other));
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>& HashMap<TKey, TValue, TComparator>::operator=(const HashMap& other)
    {
        if (this != &other)
        {
            Clear();
            CopyData(other, other.Count());
        }

        return *this;
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>& HashMap<TKey, TValue, TComparator>::operator=(HashMap&& other) noexcept
    {
        if (this != &other)
        {
            Clear();
            MoveMap(Move(other));
        }

        return *this;
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::~HashMap()
    {
        Clear();
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr TValue& HashMap<TKey, TValue, TComparator>::Add(const TKey& key, const TValue& value)
    {
        // Grow if needed.
        if (m_count >= m_buckets.Count() * kLoadFactor)
        {
            ResizeBuckets(m_buckets.Count() * kGrowMultiplier);
        }

        Bucket& bucket = GetBucket(key);

        // Check if the key already exists. If it does, update the value and return it
        for (Data& element : bucket)
        {
            if (kComparator(element.first, key))
            {
                return element.second;
            }
        }

        // If the key does not exist, add and return it
        ++m_count;
        Data& inserted = bucket.EmplaceBack(Data{ key, value });
        return inserted.second;
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr TValue& HashMap<TKey, TValue, TComparator>::Add(const Data& element)
    {
        return Add(element.first, element.second);
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::TValue& HashMap<TKey, TValue, TComparator>::Add(TKey&& key, TValue&& value)
    {
        // Grow if needed.
        if (m_count >= m_buckets.Count() * kLoadFactor)
        {
            ResizeBuckets(m_buckets.Count() * kGrowMultiplier);
        }

        Bucket& bucket = GetBucket(key);

        // Check if the key already exists. If it does, update the value and return it
        for (Data& element : bucket)
        {
            if (kComparator(element.first, key))
            {
                return element.second;
            }
        }

        // If the key does not exist, add and return it
        ++m_count;
        Data& inserted = bucket.EmplaceBack(Data{ Move(key), Move(value) });
        return inserted.second;
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr TValue& HashMap<TKey, TValue, TComparator>::Add(Data&& element)
    {
        return Emplace(Move(element.first), Move(element.second));
    }

    template<typename TKey, typename TValue, typename TComparator>
    template<typename ...TArgs>
    constexpr TValue& HashMap<TKey, TValue, TComparator>::Emplace(const TKey& key, TArgs && ...args)
    {
        // Grow if needed.
        if (m_count >= m_buckets.Count() * kLoadFactor)
        {
            ResizeBuckets(m_buckets.Count() * kGrowMultiplier);
        }

        Bucket& bucket = GetBucket(key);

        // Check if the key already exists. If it does, update the value and return it
        for (Data& element : bucket)
        {
            if (kComparator(element.first, key))
            {
                return element.second;
            }
        }

        // If the key does not exist, add and return it
        ++m_count;
        Data& inserted = bucket.EmplaceBack(Data{ key, TValue(Forward<TArgs>(args)...) });
        return inserted.second;
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::Iterator HashMap<TKey, TValue, TComparator>::Erase(const TKey& key)
    {
        if (Iterator itr = Find(key); itr != end())
        {
            Iterator nextItr = itr + 1;

            Bucket& bucket = m_buckets[itr.GetIndex()];
            bucket.Erase(itr.GetIterator());
            --m_count;

            return nextItr;
        }

        return end();
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::Iterator HashMap<TKey, TValue, TComparator>::Erase(const Iterator& iterator)
    {
        return Erase(iterator->first);
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr void HashMap<TKey, TValue, TComparator>::Clear()
    {
        m_buckets.Clear();
        m_count = 0;
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::TValue& HashMap<TKey, TValue, TComparator>::operator[](const TKey& key)
    {
        Iterator itr = Find(key);
        if (itr == end())
        {
            return Add(key, TValue());
        }

        return itr->second;
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr const HashMap<TKey, TValue, TComparator>::TValue& HashMap<TKey, TValue, TComparator>::operator[](const TKey& key) const
    {
        ConstIterator itr = Find(key);
        JPT_ASSERT(itr != cend());
        return itr->second;
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::Iterator HashMap<TKey, TValue, TComparator>::begin() noexcept
    {
        if (IsEmpty())
        {
            return end();
        }
        return Iterator(&m_buckets, 0, m_buckets.Front().begin());
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::Iterator HashMap<TKey, TValue, TComparator>::end() noexcept
    {
        return Iterator(&m_buckets, m_buckets.Count(), nullptr);
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::ConstIterator HashMap<TKey, TValue, TComparator>::begin() const noexcept
    {
        if (IsEmpty())
        {
            return end();
        }
        return ConstIterator(&m_buckets, 0, m_buckets.Front().begin());
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::ConstIterator HashMap<TKey, TValue, TComparator>::end() const noexcept
    {
        return ConstIterator(&m_buckets, m_buckets.Count(), nullptr);
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::ConstIterator HashMap<TKey, TValue, TComparator>::cbegin() const noexcept
    {
        if (IsEmpty())
        {
            return cend();
        }
        return ConstIterator(&m_buckets, 0, m_buckets.Front().cbegin());
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::ConstIterator HashMap<TKey, TValue, TComparator>::cend() const noexcept
    {
        return ConstIterator(&m_buckets, m_buckets.Count(), nullptr);
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::Iterator HashMap<TKey, TValue, TComparator>::Find(const TKey& key)
    {
        if (IsEmpty())
        {
            return end();
        }

        const Index index = GetBucketIndex(key);
        Bucket& bucket = m_buckets[index];

        for (typename Bucket::Iterator itr = bucket.begin(); itr != bucket.end(); ++itr)
        {
            if (kComparator(itr->first, key))
            {
                return Iterator(&m_buckets, index, itr);
            }
        }

        return end();
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::ConstIterator HashMap<TKey, TValue, TComparator>::Find(const TKey& key) const
    {
        if (IsEmpty())
        {
            return cend();
        }

        const Index index = GetBucketIndex(key);
        const Bucket& bucket = m_buckets[index];

        for (typename Bucket::ConstIterator itr = bucket.cbegin(); itr != bucket.cend(); ++itr)
        {
            if (kComparator(itr->first, key))
            {
                return ConstIterator(&m_buckets, index, itr);
            }
        }

        return cend();
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr bool HashMap<TKey, TValue, TComparator>::Has(const TKey& key) const
    {
        return Find(key) != cend();
    }

    template<typename TKey, typename TValue, typename TComparator>
    void HashMap<TKey, TValue, TComparator>::Serialize(Serializer& serializer) const
    {
        serializer.Write(m_count);

        for (const auto& [key, value] : *this)
        {
            serializer.Write(key);
            serializer.Write(value);
        }
    }

    template<typename TKey, typename TValue, typename TComparator>
    void HashMap<TKey, TValue, TComparator>::Deserialize(Serializer& serializer)
    {
        Clear();

        Index count;
        serializer.Read(count);

        for (Index i = 0; i < count; ++i)
        {
            TKey key;
            TValue value;

            serializer.Read(key);
            serializer.Read(value);

            Add(key, value);
        }
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr Index HashMap<TKey, TValue, TComparator>::Count() const noexcept
    {
        return m_count;
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr bool HashMap<TKey, TValue, TComparator>::IsEmpty() const noexcept
    {
        return m_count == 0;
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr void HashMap<TKey, TValue, TComparator>::ResizeBuckets(Index capacity)
    {
        static constexpr Index kMinCapacity = 16;

        Buckets newBuckets;
        newBuckets.Resize(Max(kMinCapacity, capacity));

        for (const Bucket& bucket : m_buckets)
        {
            for (const Data& element : bucket)
            {
                const Index index = GetBucketIndex(element.first, newBuckets.Count());
                newBuckets[index].EmplaceBack(Move(element));
            }
        }

        m_buckets = Move(newBuckets);
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr Index HashMap<TKey, TValue, TComparator>::GetBucketIndex(const TKey& key) const
    {
        return Hash(key) % m_buckets.Count();
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr Index HashMap<TKey, TValue, TComparator>::GetBucketIndex(const TKey& key, Index bucketCount) const
    {
        return Hash(key) % bucketCount;
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr HashMap<TKey, TValue, TComparator>::Bucket& HashMap<TKey, TValue, TComparator>::GetBucket(const TKey& key)
    {
        return m_buckets[GetBucketIndex(key)];
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr const HashMap<TKey, TValue, TComparator>::Bucket& HashMap<TKey, TValue, TComparator>::GetBucket(const TKey& key) const
    {
        return m_buckets[GetBucketIndex(key)];
    }

    template<typename TKey, typename TValue, typename TComparator>
    template<Iterable TContainer>
    constexpr void HashMap<TKey, TValue, TComparator>::CopyData(const TContainer& container, Index size)
    {
        ResizeBuckets(m_count + size);

        for (const Data& element : container)
        {
            Add(element);
        }
    }

    template<typename TKey, typename TValue, typename TComparator>
    constexpr void HashMap<TKey, TValue, TComparator>::MoveMap(HashMap&& other)
    {
        m_buckets = Move(other.m_buckets);
        m_count   = other.m_count;

        other.m_count = 0;
    }
}
