// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.Iterator_HashTable_Chaining;

import jpt.TypeDefs;
import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;

import jpt_private.Iterator_LinearNode;

// Serves both HashSet and HashMap

export namespace jpt_private
{
    template<typename TData>
    class Iterator_HashTable_Chaining
    {
    public:
        using TBucket  = jpt::LinkedList<TData>;
        using TBuckets = jpt::DynamicArray<TBucket>;

    private:
        Iterator_LinearNode<TData> m_iterator;
        TBuckets* m_pBuckets = nullptr;
        size_t m_index = 0;

    public:
        constexpr Iterator_HashTable_Chaining() = default;
        constexpr Iterator_HashTable_Chaining(TBuckets* pBuckets, size_t index, Iterator_LinearNode<TData> iterator);

        constexpr Iterator_HashTable_Chaining& operator++();
        constexpr Iterator_HashTable_Chaining operator++(int32);

        constexpr Iterator_HashTable_Chaining& operator+=(size_t offset);
        constexpr Iterator_HashTable_Chaining operator+(size_t offset);

        constexpr       TData* operator->()       { return &m_iterator; }
        constexpr const TData* operator->() const { return &m_iterator; }
        constexpr       TData& operator*()        { return *m_iterator; }
        constexpr const TData& operator*()  const { return *m_iterator; }

        constexpr       Iterator_LinearNode<TData>& GetIterator()       { return m_iterator; }
        constexpr const Iterator_LinearNode<TData>& GetIterator() const { return m_iterator; }

        constexpr size_t GetIndex() const { return m_index; }

        constexpr bool operator==(const Iterator_HashTable_Chaining& other) const;

    private:
        constexpr bool HasReachedEnd() const;
        constexpr void FindNextValidIterator();
    };

    template<typename TData>
    constexpr Iterator_HashTable_Chaining<TData>::Iterator_HashTable_Chaining(TBuckets* pBuckets, size_t index, Iterator_LinearNode<TData> iterator)
        : m_pBuckets(pBuckets)
        , m_index(index)
        , m_iterator(iterator)
    {
        FindNextValidIterator();
    }

    template<typename TData>
    constexpr Iterator_HashTable_Chaining<TData>& Iterator_HashTable_Chaining<TData>::operator++()
    {
        ++m_iterator;
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr Iterator_HashTable_Chaining<TData> Iterator_HashTable_Chaining<TData>::operator++(int32)
    {
        Iterator_HashTable_Chaining iterator = *this;
        ++m_iterator;
        FindNextValidIterator();
        return iterator;
    }

    template<typename TData>
    constexpr Iterator_HashTable_Chaining<TData>& Iterator_HashTable_Chaining<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++m_iterator;
            FindNextValidIterator();
        }
        return *this;
    }

    template<typename TData>
    constexpr Iterator_HashTable_Chaining<TData> Iterator_HashTable_Chaining<TData>::operator+(size_t offset)
    {
        Iterator_HashTable_Chaining iterator = *this;
        return iterator += offset;
    }

    template<typename TData>
    constexpr bool Iterator_HashTable_Chaining<TData>::operator==(const Iterator_HashTable_Chaining& other) const
    {
        return m_pBuckets == other.m_pBuckets &&
               m_index    == other.m_index    &&
               m_iterator == other.m_iterator;
    }

    template<typename TData>
    constexpr bool Iterator_HashTable_Chaining<TData>::HasReachedEnd() const
    {
        return m_index == m_pBuckets->Count();
    }

    template<typename TData>
    constexpr void Iterator_HashTable_Chaining<TData>::FindNextValidIterator()
    {
        while (!m_iterator.GetNode() && !HasReachedEnd())
        {
            ++m_index;

            if (HasReachedEnd())
            {
                m_iterator = Iterator_LinearNode<TData>(nullptr);
                return;
            }

            TBucket& pBucket = m_pBuckets->At(m_index);
            m_iterator = pBucket.begin();
        }
    }

    template<typename TData>
    class ConstIterator_HashTable_Chaining
    {
        using TBucket = jpt::LinkedList<TData>;
        using TBuckets = jpt::DynamicArray<TBucket>;

    private:
        ConstIterator_LinearNode<TData> m_iterator;
        const TBuckets* m_pBuckets = nullptr;
        size_t m_index = 0;

    public:
        constexpr ConstIterator_HashTable_Chaining() = default;
        constexpr ConstIterator_HashTable_Chaining(const TBuckets* pBuckets, size_t index, ConstIterator_LinearNode<TData> iterator);

        constexpr ConstIterator_HashTable_Chaining& operator++();
        constexpr ConstIterator_HashTable_Chaining operator++(int32);

        constexpr ConstIterator_HashTable_Chaining& operator+=(size_t offset);
        constexpr ConstIterator_HashTable_Chaining operator+(size_t offset);

        constexpr const TData* operator->() const { return &m_iterator; }
        constexpr const TData& operator*()  const { return *m_iterator; }

        constexpr bool operator==(const ConstIterator_HashTable_Chaining& other) const;

    private:
        constexpr bool HasReachedEnd() const;
        constexpr void FindNextValidIterator();
    };

    template<typename TData>
    constexpr ConstIterator_HashTable_Chaining<TData>::ConstIterator_HashTable_Chaining(const TBuckets* pBuckets, size_t index, ConstIterator_LinearNode<TData> iterator)
        : m_pBuckets(pBuckets)
        , m_index(index)
        , m_iterator(iterator)
    {
        FindNextValidIterator();
    }

    template<typename TData>
    constexpr ConstIterator_HashTable_Chaining<TData>& ConstIterator_HashTable_Chaining<TData>::operator++()
    {
        ++m_iterator;
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr ConstIterator_HashTable_Chaining<TData> ConstIterator_HashTable_Chaining<TData>::operator++(int32)
    {
        ConstIterator_HashTable_Chaining iterator = *this;
        ++m_iterator;
        FindNextValidIterator();
        return iterator;
    }

    template<typename TData>
    constexpr ConstIterator_HashTable_Chaining<TData>& ConstIterator_HashTable_Chaining<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++m_iterator;
            FindNextValidIterator();
        }
        return *this;
    }

    template<typename TData>
    constexpr ConstIterator_HashTable_Chaining<TData> ConstIterator_HashTable_Chaining<TData>::operator+(size_t offset)
    {
        ConstIterator_HashTable_Chaining iterator = *this;
        return iterator += offset;
    }

    template<typename TData>
    constexpr bool ConstIterator_HashTable_Chaining<TData>::operator==(const ConstIterator_HashTable_Chaining& other) const
    {
        return m_pBuckets == other.m_pBuckets &&
               m_index    == other.m_index    &&
               m_iterator == other.m_iterator;
    }

    template<typename TData>
    constexpr bool ConstIterator_HashTable_Chaining<TData>::HasReachedEnd() const
    {
        return m_index == m_pBuckets->Count();
    }

    template<typename TData>
    constexpr void ConstIterator_HashTable_Chaining<TData>::FindNextValidIterator()
    {
        while (!m_iterator.GetNode() && !HasReachedEnd())
        {
            ++m_index;

            if (HasReachedEnd())
            {
                m_iterator = ConstIterator_LinearNode<TData>(nullptr);
                return;
            }

            const TBucket& pBucket = m_pBuckets->At(m_index);
            m_iterator = pBucket.cbegin();
        }
    }
}