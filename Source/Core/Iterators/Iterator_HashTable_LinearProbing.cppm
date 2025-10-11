// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.Iterator_HashTable_LinearProbing;

import jpt.TypeDefs;
import jpt.Pair;

export namespace jpt_private
{
    template<typename TKey, typename TValue>
    struct Entry
    {
        using TData = jpt::Pair<TKey, TValue>;

        TData data;
        bool isOccupied = false;
    };

    template<typename TKey, typename TValue, Index kCapacity>
    class Iterator_HashTable_LinearProbing
    {
    private:
        Entry<TKey, TValue>* m_pArray;
        Index m_index = 0;  /**< Current index in the hash table */

    public:
        constexpr Iterator_HashTable_LinearProbing() = default;
        constexpr Iterator_HashTable_LinearProbing(Entry<TKey, TValue>* pArray, Index index);

        constexpr Iterator_HashTable_LinearProbing& operator++();
        constexpr Iterator_HashTable_LinearProbing operator++(int32);

        constexpr Iterator_HashTable_LinearProbing& operator+=(Index offset);
        constexpr Iterator_HashTable_LinearProbing operator+(Index offset);

        constexpr       Entry<TKey, TValue>::TData* operator->()       { return &m_pArray[m_index].data; }
        constexpr const Entry<TKey, TValue>::TData* operator->() const { return &m_pArray[m_index].data; }
        constexpr       Entry<TKey, TValue>::TData& operator*()        { return  m_pArray[m_index].data; }
        constexpr const Entry<TKey, TValue>::TData& operator*()  const { return  m_pArray[m_index].data; }

        constexpr Index GetIndex() const { return m_index; }

        constexpr bool operator==(const Iterator_HashTable_LinearProbing& other) const;

    private:
        constexpr void FindNextValidIndex();
    };

    template<typename TKey, typename TValue, Index kCapacity>
    class ConstIterator_HashTable_LinearProbing
    {
    private:
        const Entry<TKey, TValue>* m_pArray;
        Index m_index = 0;  /**< Current index in the hash table */

    public:
        constexpr ConstIterator_HashTable_LinearProbing() = default;
        constexpr ConstIterator_HashTable_LinearProbing(const Entry<TKey, TValue>* pArray, Index index);

        constexpr ConstIterator_HashTable_LinearProbing& operator++();
        constexpr ConstIterator_HashTable_LinearProbing operator++(int32);

        constexpr ConstIterator_HashTable_LinearProbing& operator+=(Index offset);
        constexpr ConstIterator_HashTable_LinearProbing operator+(Index offset);

        constexpr const Entry<TKey, TValue>::TData* operator->() const { return &m_pArray[m_index].data; }
        constexpr const Entry<TKey, TValue>::TData& operator*()  const { return  m_pArray[m_index].data; }

        constexpr Index GetIndex() const { return m_index; }

        constexpr bool operator==(const ConstIterator_HashTable_LinearProbing& other) const;

    private:
        constexpr void FindNextValidIndex();
    };

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr Iterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::Iterator_HashTable_LinearProbing(Entry<TKey, TValue>* pArray, Index index)
        : m_pArray(pArray)
        , m_index(index)
    {
        FindNextValidIndex();
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr Iterator_HashTable_LinearProbing<TKey, TValue, kCapacity>& Iterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::operator++()
    {
        ++m_index;
        FindNextValidIndex();
        return *this;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr Iterator_HashTable_LinearProbing<TKey, TValue, kCapacity> Iterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::operator++(int32)
    {
        Iterator_HashTable_LinearProbing iterator = *this;
        ++m_index;
        FindNextValidIndex();
        return iterator;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr Iterator_HashTable_LinearProbing<TKey, TValue, kCapacity>& Iterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::operator+=(Index offset)
    {
        for (Index i = 0; i < offset; ++i)
        {
            ++m_index;
            FindNextValidIndex();
        }
        return *this;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr Iterator_HashTable_LinearProbing<TKey, TValue, kCapacity> Iterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::operator+(Index offset)
    {
        Iterator_HashTable_LinearProbing iterator = *this;
        return iterator += offset;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr bool Iterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::operator==(const Iterator_HashTable_LinearProbing& other) const
    {
        return m_pArray == other.m_pArray &&
               m_index    == other.m_index;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr void Iterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::FindNextValidIndex()
    {
        while (m_index < kCapacity &&
              !m_pArray[m_index].isOccupied)
        {
            ++m_index;
        }
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr ConstIterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::ConstIterator_HashTable_LinearProbing(const Entry<TKey, TValue>* pArray, Index index)
        : m_pArray(pArray)
        , m_index(index)
    {
        FindNextValidIndex();
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr ConstIterator_HashTable_LinearProbing<TKey, TValue, kCapacity>& ConstIterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::operator++()
    {
        ++m_index;
        FindNextValidIndex();
        return *this;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr ConstIterator_HashTable_LinearProbing<TKey, TValue, kCapacity> ConstIterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::operator++(int32)
    {
        ConstIterator_HashTable_LinearProbing iterator = *this;
        ++m_index;
        FindNextValidIndex();
        return iterator;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr ConstIterator_HashTable_LinearProbing<TKey, TValue, kCapacity>& ConstIterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::operator+=(Index offset)
    {
        for (Index i = 0; i < offset; ++i)
        {
            ++m_index;
            FindNextValidIndex();
        }
        return *this;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr ConstIterator_HashTable_LinearProbing<TKey, TValue, kCapacity> ConstIterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::operator+(Index offset)
    {
        ConstIterator_HashTable_LinearProbing iterator = *this;
        return iterator += offset;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr bool ConstIterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::operator==(const ConstIterator_HashTable_LinearProbing& other) const
    {
        return m_pArray == other.m_pArray &&
               m_index    == other.m_index;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr void ConstIterator_HashTable_LinearProbing<TKey, TValue, kCapacity>::FindNextValidIndex()
    {
        while (m_index < kCapacity &&
              !m_pArray[m_index].isOccupied)
        {
            ++m_index;
        }
    }
}