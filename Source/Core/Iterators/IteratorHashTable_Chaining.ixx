// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.IteratorHashTable_Chaining;

import jpt.TypeDefs;
import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;

import jpt_private.IteratorLinearNode;

// Serves both HashSet and HashMap

export namespace jpt_private
{
    template<typename TData>
    class IteratorHashTable_Chaining
    {
    public:
        using TBucket  = jpt::LinkedList<TData>;
        using TBuckets = jpt::DynamicArray<TBucket>;

    private:
        IteratorLinearNode<TData> m_iterator;
        TBuckets* m_pBuckets = nullptr;
        size_t m_index = 0;

    public:
        constexpr IteratorHashTable_Chaining() = default;
        constexpr IteratorHashTable_Chaining(TBuckets* pBuckets, size_t index, IteratorLinearNode<TData> iterator);

        constexpr IteratorHashTable_Chaining& operator++();
        constexpr IteratorHashTable_Chaining operator++(int32);

        constexpr IteratorHashTable_Chaining& operator+=(size_t offset);
        constexpr IteratorHashTable_Chaining operator+(size_t offset);

        constexpr       TData* operator->()       { return &m_iterator; }
        constexpr const TData* operator->() const { return &m_iterator; }
        constexpr       TData& operator*()        { return *m_iterator; }
        constexpr const TData& operator*()  const { return *m_iterator; }

        constexpr       IteratorLinearNode<TData>& GetIterator()       { return m_iterator; }
        constexpr const IteratorLinearNode<TData>& GetIterator() const { return m_iterator; }

        constexpr size_t GetIndex() const { return m_index; }

        constexpr bool operator==(const IteratorHashTable_Chaining& other) const;

    private:
        constexpr bool HasReachedEnd() const;
        constexpr void FindNextValidIterator();
    };

    template<typename TData>
    constexpr IteratorHashTable_Chaining<TData>::IteratorHashTable_Chaining(TBuckets* pBuckets, size_t index, IteratorLinearNode<TData> iterator)
        : m_pBuckets(pBuckets)
        , m_index(index)
        , m_iterator(iterator)
    {
        FindNextValidIterator();
    }

    template<typename TData>
    constexpr IteratorHashTable_Chaining<TData>& IteratorHashTable_Chaining<TData>::operator++()
    {
        ++m_iterator;
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr IteratorHashTable_Chaining<TData> IteratorHashTable_Chaining<TData>::operator++(int32)
    {
        IteratorHashTable_Chaining iterator = *this;
        ++m_iterator;
        FindNextValidIterator();
        return iterator;
    }

    template<typename TData>
    constexpr IteratorHashTable_Chaining<TData>& IteratorHashTable_Chaining<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++m_iterator;
            FindNextValidIterator();
        }
        return *this;
    }

    template<typename TData>
    constexpr IteratorHashTable_Chaining<TData> IteratorHashTable_Chaining<TData>::operator+(size_t offset)
    {
        IteratorHashTable_Chaining iterator = *this;
        return iterator += offset;
    }

    template<typename TData>
    constexpr bool IteratorHashTable_Chaining<TData>::operator==(const IteratorHashTable_Chaining& other) const
    {
        return m_pBuckets == other.m_pBuckets &&
               m_index    == other.m_index    &&
               m_iterator == other.m_iterator;
    }

    template<typename TData>
    constexpr bool IteratorHashTable_Chaining<TData>::HasReachedEnd() const
    {
        return m_index == m_pBuckets->Count();
    }

    template<typename TData>
    constexpr void IteratorHashTable_Chaining<TData>::FindNextValidIterator()
    {
        while (!m_iterator.GetNode() && !HasReachedEnd())
        {
            ++m_index;

            if (HasReachedEnd())
            {
                m_iterator = IteratorLinearNode<TData>(nullptr);
                return;
            }

            TBucket& pBucket = m_pBuckets->At(m_index);
            m_iterator = pBucket.begin();
        }
    }

    template<typename TData>
    class ConstIteratorHashTable_Chaining
    {
        using TBucket = jpt::LinkedList<TData>;
        using TBuckets = jpt::DynamicArray<TBucket>;

    private:
        ConstIteratorLinearNode<TData> m_iterator;
        const TBuckets* m_pBuckets = nullptr;
        size_t m_index = 0;

    public:
        constexpr ConstIteratorHashTable_Chaining() = default;
        constexpr ConstIteratorHashTable_Chaining(const TBuckets* pBuckets, size_t index, ConstIteratorLinearNode<TData> iterator);

        constexpr ConstIteratorHashTable_Chaining& operator++();
        constexpr ConstIteratorHashTable_Chaining operator++(int32);

        constexpr ConstIteratorHashTable_Chaining& operator+=(size_t offset);
        constexpr ConstIteratorHashTable_Chaining operator+(size_t offset);

        constexpr const TData* operator->() const { return &m_iterator; }
        constexpr const TData& operator*()  const { return *m_iterator; }

        constexpr bool operator==(const ConstIteratorHashTable_Chaining& other) const;

    private:
        constexpr bool HasReachedEnd() const;
        constexpr void FindNextValidIterator();
    };

    template<typename TData>
    constexpr ConstIteratorHashTable_Chaining<TData>::ConstIteratorHashTable_Chaining(const TBuckets* pBuckets, size_t index, ConstIteratorLinearNode<TData> iterator)
        : m_pBuckets(pBuckets)
        , m_index(index)
        , m_iterator(iterator)
    {
        FindNextValidIterator();
    }

    template<typename TData>
    constexpr ConstIteratorHashTable_Chaining<TData>& ConstIteratorHashTable_Chaining<TData>::operator++()
    {
        ++m_iterator;
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr ConstIteratorHashTable_Chaining<TData> ConstIteratorHashTable_Chaining<TData>::operator++(int32)
    {
        ConstIteratorHashTable_Chaining iterator = *this;
        ++m_iterator;
        FindNextValidIterator();
        return iterator;
    }

    template<typename TData>
    constexpr ConstIteratorHashTable_Chaining<TData>& ConstIteratorHashTable_Chaining<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++m_iterator;
            FindNextValidIterator();
        }
        return *this;
    }

    template<typename TData>
    constexpr ConstIteratorHashTable_Chaining<TData> ConstIteratorHashTable_Chaining<TData>::operator+(size_t offset)
    {
        ConstIteratorHashTable_Chaining iterator = *this;
        return iterator += offset;
    }

    template<typename TData>
    constexpr bool ConstIteratorHashTable_Chaining<TData>::operator==(const ConstIteratorHashTable_Chaining& other) const
    {
        return m_pBuckets == other.m_pBuckets &&
               m_index    == other.m_index    &&
               m_iterator == other.m_iterator;
    }

    template<typename TData>
    constexpr bool ConstIteratorHashTable_Chaining<TData>::HasReachedEnd() const
    {
        return m_index == m_pBuckets->Count();
    }

    template<typename TData>
    constexpr void ConstIteratorHashTable_Chaining<TData>::FindNextValidIterator()
    {
        while (!m_iterator.GetNode() && !HasReachedEnd())
        {
            ++m_index;

            if (HasReachedEnd())
            {
                m_iterator = ConstIteratorLinearNode<TData>(nullptr);
                return;
            }

            const TBucket& pBucket = m_pBuckets->At(m_index);
            m_iterator = pBucket.cbegin();
        }
    }
}