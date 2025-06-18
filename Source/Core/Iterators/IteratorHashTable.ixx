// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.IteratorHashTable;

import jpt.TypeDefs;
import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;

import jpt_private.IteratorLinearNode;

// Serves both HashSet and HashMap

export namespace jpt_private
{
    template<typename TData>
    class IteratorHashTable
    {
    public:
        using TBucket  = jpt::LinkedList<TData>;
        using TBuckets = jpt::DynamicArray<TBucket>;

    private:
        IteratorLinearNode<TData> m_iterator;
        TBuckets* m_pBuckets = nullptr;
        size_t m_index = 0;

    public:
        constexpr IteratorHashTable() = default;
        constexpr IteratorHashTable(TBuckets* pBuckets, size_t index, IteratorLinearNode<TData> iterator);

        constexpr IteratorHashTable& operator++();
        constexpr IteratorHashTable operator++(int32);

        constexpr IteratorHashTable& operator+=(size_t offset);
        constexpr IteratorHashTable operator+(size_t offset);

        constexpr       TData* operator->()       { return &m_iterator; }
        constexpr const TData* operator->() const { return &m_iterator; }
        constexpr       TData& operator*()        { return *m_iterator; }
        constexpr const TData& operator*()  const { return *m_iterator; }

        constexpr       IteratorLinearNode<TData>& GetIterator()       { return m_iterator; }
        constexpr const IteratorLinearNode<TData>& GetIterator() const { return m_iterator; }

        constexpr size_t GetIndex() const { return m_index; }

        constexpr bool operator==(const IteratorHashTable& other) const;

    private:
        constexpr bool HasReachedEnd() const;
        constexpr void FindNextValidIterator();
    };

    template<typename TData>
    constexpr IteratorHashTable<TData>::IteratorHashTable(TBuckets* pBuckets, size_t index, IteratorLinearNode<TData> iterator)
        : m_pBuckets(pBuckets)
        , m_index(index)
        , m_iterator(iterator)
    {
        FindNextValidIterator();
    }

    template<typename TData>
    constexpr IteratorHashTable<TData>& IteratorHashTable<TData>::operator++()
    {
        ++m_iterator;
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr IteratorHashTable<TData> IteratorHashTable<TData>::operator++(int32)
    {
        IteratorHashTable iterator = *this;
        ++m_iterator;
        FindNextValidIterator();
        return iterator;
    }

    template<typename TData>
    constexpr IteratorHashTable<TData>& IteratorHashTable<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++m_iterator;
            FindNextValidIterator();
        }
        return *this;
    }

    template<typename TData>
    constexpr IteratorHashTable<TData> IteratorHashTable<TData>::operator+(size_t offset)
    {
        IteratorHashTable iterator = *this;
        return iterator += offset;
    }

    template<typename TData>
    constexpr bool IteratorHashTable<TData>::operator==(const IteratorHashTable& other) const
    {
        return m_pBuckets == other.m_pBuckets &&
               m_index    == other.m_index    &&
               m_iterator == other.m_iterator;
    }

    template<typename TData>
    constexpr bool IteratorHashTable<TData>::HasReachedEnd() const
    {
        return m_index == m_pBuckets->Count();
    }

    template<typename TData>
    constexpr void IteratorHashTable<TData>::FindNextValidIterator()
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
    class ConstIteratorHashTable
    {
        using TBucket = jpt::LinkedList<TData>;
        using TBuckets = jpt::DynamicArray<TBucket>;

    private:
        ConstIteratorLinearNode<TData> m_iterator;
        const TBuckets* m_pBuckets = nullptr;
        size_t m_index = 0;

    public:
        constexpr ConstIteratorHashTable() = default;
        constexpr ConstIteratorHashTable(const TBuckets* pBuckets, size_t index, ConstIteratorLinearNode<TData> iterator);

        constexpr ConstIteratorHashTable& operator++();
        constexpr ConstIteratorHashTable operator++(int32);

        constexpr ConstIteratorHashTable& operator+=(size_t offset);
        constexpr ConstIteratorHashTable operator+(size_t offset);

        constexpr const TData* operator->() const { return &m_iterator; }
        constexpr const TData& operator*()  const { return *m_iterator; }

        constexpr bool operator==(const ConstIteratorHashTable& other) const;

    private:
        constexpr bool HasReachedEnd() const;
        constexpr void FindNextValidIterator();
    };

    template<typename TData>
    constexpr ConstIteratorHashTable<TData>::ConstIteratorHashTable(const TBuckets* pBuckets, size_t index, ConstIteratorLinearNode<TData> iterator)
        : m_pBuckets(pBuckets)
        , m_index(index)
        , m_iterator(iterator)
    {
        FindNextValidIterator();
    }

    template<typename TData>
    constexpr ConstIteratorHashTable<TData>& ConstIteratorHashTable<TData>::operator++()
    {
        ++m_iterator;
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr ConstIteratorHashTable<TData> ConstIteratorHashTable<TData>::operator++(int32)
    {
        ConstIteratorHashTable iterator = *this;
        ++m_iterator;
        FindNextValidIterator();
        return iterator;
    }

    template<typename TData>
    constexpr ConstIteratorHashTable<TData>& ConstIteratorHashTable<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++m_iterator;
            FindNextValidIterator();
        }
        return *this;
    }

    template<typename TData>
    constexpr ConstIteratorHashTable<TData> ConstIteratorHashTable<TData>::operator+(size_t offset)
    {
        ConstIteratorHashTable iterator = *this;
        return iterator += offset;
    }

    template<typename TData>
    constexpr bool ConstIteratorHashTable<TData>::operator==(const ConstIteratorHashTable& other) const
    {
        return m_pBuckets == other.m_pBuckets &&
               m_index    == other.m_index    &&
               m_iterator == other.m_iterator;
    }

    template<typename TData>
    constexpr bool ConstIteratorHashTable<TData>::HasReachedEnd() const
    {
        return m_index == m_pBuckets->Count();
    }

    template<typename TData>
    constexpr void ConstIteratorHashTable<TData>::FindNextValidIterator()
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