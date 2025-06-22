// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.IteratorHashTable_LinearProbing;

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
    class IteratorHashTable_LinearProbing
    {
    private:
        Entry<TKey, TValue>* m_pArray;
        Index m_index = 0;  /**< Current index in the hash table */

    public:
        constexpr IteratorHashTable_LinearProbing() = default;
        constexpr IteratorHashTable_LinearProbing(Entry<TKey, TValue>* pArray, Index index);

        constexpr IteratorHashTable_LinearProbing& operator++();
        constexpr IteratorHashTable_LinearProbing operator++(int32);

        constexpr IteratorHashTable_LinearProbing& operator+=(Index offset);
        constexpr IteratorHashTable_LinearProbing operator+(Index offset);

        constexpr       Entry<TKey, TValue>::TData* operator->()       { return &m_pArray[m_index].data; }
        constexpr const Entry<TKey, TValue>::TData* operator->() const { return &m_pArray[m_index].data; }
        constexpr       Entry<TKey, TValue>::TData& operator*()        { return  m_pArray[m_index].data; }
        constexpr const Entry<TKey, TValue>::TData& operator*()  const { return  m_pArray[m_index].data; }

        constexpr bool operator==(const IteratorHashTable_LinearProbing& other) const;

    private:
        constexpr void FindNextValidIndex();
    };

    template<typename TKey, typename TValue, Index kCapacity>
    class ConstIteratorHashTable_LinearProbing
    {
    private:
        const Entry<TKey, TValue>* m_pArray;
        Index m_index = 0;  /**< Current index in the hash table */

    public:
        constexpr ConstIteratorHashTable_LinearProbing() = default;
        constexpr ConstIteratorHashTable_LinearProbing(const Entry<TKey, TValue>* pArray, Index index);

        constexpr ConstIteratorHashTable_LinearProbing& operator++();
        constexpr ConstIteratorHashTable_LinearProbing operator++(int32);

        constexpr ConstIteratorHashTable_LinearProbing& operator+=(Index offset);
        constexpr ConstIteratorHashTable_LinearProbing operator+(Index offset);

        constexpr const Entry<TKey, TValue>::TData* operator->() const { return &m_pArray[m_index].data; }
        constexpr const Entry<TKey, TValue>::TData& operator*()  const { return  m_pArray[m_index].data; }

        constexpr bool operator==(const ConstIteratorHashTable_LinearProbing& other) const;

    private:
        constexpr void FindNextValidIndex();
    };

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr IteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::IteratorHashTable_LinearProbing(Entry<TKey, TValue>* pArray, Index index)
        : m_pArray(pArray)
        , m_index(index)
    {
        FindNextValidIndex();
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr IteratorHashTable_LinearProbing<TKey, TValue, kCapacity>& IteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::operator++()
    {
        ++m_index;
        FindNextValidIndex();
        return *this;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr IteratorHashTable_LinearProbing<TKey, TValue, kCapacity> IteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::operator++(int32)
    {
        IteratorHashTable_LinearProbing iterator = *this;
        ++m_index;
        FindNextValidIndex();
        return iterator;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr IteratorHashTable_LinearProbing<TKey, TValue, kCapacity>& IteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::operator+=(Index offset)
    {
        for (Index i = 0; i < offset; ++i)
        {
            ++m_index;
            FindNextValidIndex();
        }
        return *this;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr IteratorHashTable_LinearProbing<TKey, TValue, kCapacity> IteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::operator+(Index offset)
    {
        IteratorHashTable_LinearProbing iterator = *this;
        return iterator += offset;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr bool IteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::operator==(const IteratorHashTable_LinearProbing& other) const
    {
        return m_pArray == other.m_pArray &&
               m_index    == other.m_index;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr void IteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::FindNextValidIndex()
    {
        while (m_index < kCapacity &&
              !m_pArray[m_index].isOccupied)
        {
            ++m_index;
        }
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::ConstIteratorHashTable_LinearProbing(const Entry<TKey, TValue>* pArray, Index index)
        : m_pArray(pArray)
        , m_index(index)
    {
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity>& ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::operator++()
    {
        ++m_index;
        FindNextValidIndex();
        return *this;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity> ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::operator++(int32)
    {
        ConstIteratorHashTable_LinearProbing iterator = *this;
        ++m_index;
        FindNextValidIndex();
        return iterator;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity>& ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::operator+=(Index offset)
    {
        for (Index i = 0; i < offset; ++i)
        {
            ++m_index;
            FindNextValidIndex();
        }
        return *this;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity> ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::operator+(Index offset)
    {
        ConstIteratorHashTable_LinearProbing iterator = *this;
        return iterator += offset;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr bool ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::operator==(const ConstIteratorHashTable_LinearProbing& other) const
    {
        return m_pArray == other.m_pArray &&
               m_index    == other.m_index;
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr void ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::FindNextValidIndex()
    {
        while (m_index < kCapacity &&
              !m_pArray[m_index].isOccupied)
        {
            ++m_index;
        }
    }
}