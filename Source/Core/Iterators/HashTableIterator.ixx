// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.HashTableIterator;

import jpt.TypeDefs;
import jpt.Pair;
import jpt.DynamicArray;
import jpt.LinkedList;

import jpt_private.LinearNodeIterator;

// Serves both HashSet and HashMap

export namespace jpt_private
{
    template<typename TData>
    class HashTableIterator
    {
    public:
        using TBucket  = jpt::LinkedList<TData>;
        using TBuckets = jpt::DynamicArray<TBucket>;

    private:
        LinearNodeIterator<TData> m_iterator;
        TBuckets* m_pBuckets = nullptr;
        size_t m_index = 0;

    public:
        constexpr HashTableIterator() = default;
        constexpr HashTableIterator(TBuckets* pBuckets, size_t index, LinearNodeIterator<TData> iterator);

        constexpr HashTableIterator& operator++();
        constexpr HashTableIterator operator++(int32);

        constexpr HashTableIterator& operator+=(size_t offset);
        constexpr HashTableIterator operator+(size_t offset);

        constexpr       TData* operator->()       { return &m_iterator; }
        constexpr const TData* operator->() const { return &m_iterator; }
        constexpr       TData& operator*()        { return *m_iterator; }
        constexpr const TData& operator*()  const { return *m_iterator; }

        constexpr       LinearNodeIterator<TData>& GetIterator()       { return m_iterator; }
        constexpr const LinearNodeIterator<TData>& GetIterator() const { return m_iterator; }

        constexpr size_t GetIndex() const { return m_index; }

        constexpr bool operator==(const HashTableIterator& other) const;

    private:
        constexpr bool HasReachedEnd() const;
        constexpr void FindNextValidIterator();
    };

    template<typename TData>
    constexpr HashTableIterator<TData>::HashTableIterator(TBuckets* pBuckets, size_t index, LinearNodeIterator<TData> iterator)
        : m_pBuckets(pBuckets)
        , m_index(index)
        , m_iterator(iterator)
    {
        FindNextValidIterator();
    }

    template<typename TData>
    constexpr HashTableIterator<TData>& HashTableIterator<TData>::operator++()
    {
        ++m_iterator;
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr HashTableIterator<TData> HashTableIterator<TData>::operator++(int32)
    {
        HashTableIterator iterator = *this;
        ++m_iterator;
        FindNextValidIterator();
        return iterator;
    }

    template<typename TData>
    constexpr HashTableIterator<TData>& HashTableIterator<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++m_iterator;
            FindNextValidIterator();
        }
        return *this;
    }

    template<typename TData>
    constexpr HashTableIterator<TData> HashTableIterator<TData>::operator+(size_t offset)
    {
        HashTableIterator iterator = *this;
        return iterator += offset;
    }

    template<typename TData>
    constexpr bool HashTableIterator<TData>::operator==(const HashTableIterator& other) const
    {
        return m_pBuckets == other.m_pBuckets &&
               m_index    == other.m_index    &&
               m_iterator == other.m_iterator;
    }

    template<typename TData>
    constexpr bool HashTableIterator<TData>::HasReachedEnd() const
    {
        return m_index == m_pBuckets->Count();
    }

    template<typename TData>
    constexpr void HashTableIterator<TData>::FindNextValidIterator()
    {
        while (!m_iterator.GetNode() && !HasReachedEnd())
        {
            ++m_index;

            if (HasReachedEnd())
            {
                m_iterator = LinearNodeIterator<TData>(nullptr);
                return;
            }

            TBucket& pBucket = m_pBuckets->At(m_index);
            m_iterator = pBucket.begin();
        }
    }

    template<typename TData>
    class ConstHashTableIterator
    {
        using TBucket = jpt::LinkedList<TData>;
        using TBuckets = jpt::DynamicArray<TBucket>;

    private:
        ConstLinearNodeIterator<TData> m_iterator;
        const TBuckets* m_pBuckets = nullptr;
        size_t m_index = 0;

    public:
        constexpr ConstHashTableIterator() = default;
        constexpr ConstHashTableIterator(const TBuckets* pBuckets, size_t index, ConstLinearNodeIterator<TData> iterator);

        constexpr ConstHashTableIterator& operator++();
        constexpr ConstHashTableIterator operator++(int32);

        constexpr ConstHashTableIterator& operator+=(size_t offset);
        constexpr ConstHashTableIterator operator+(size_t offset);

        constexpr const TData* operator->() const { return &m_iterator; }
        constexpr const TData& operator*()  const { return *m_iterator; }

        constexpr bool operator==(const ConstHashTableIterator& other) const;

    private:
        constexpr bool HasReachedEnd() const;
        constexpr void FindNextValidIterator();
    };

    template<typename TData>
    constexpr ConstHashTableIterator<TData>::ConstHashTableIterator(const TBuckets* pBuckets, size_t index, ConstLinearNodeIterator<TData> iterator)
        : m_pBuckets(pBuckets)
        , m_index(index)
        , m_iterator(iterator)
    {
        FindNextValidIterator();
    }

    template<typename TData>
    constexpr ConstHashTableIterator<TData>& ConstHashTableIterator<TData>::operator++()
    {
        ++m_iterator;
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr ConstHashTableIterator<TData> ConstHashTableIterator<TData>::operator++(int32)
    {
        ConstHashTableIterator iterator = *this;
        ++m_iterator;
        FindNextValidIterator();
        return iterator;
    }

    template<typename TData>
    constexpr ConstHashTableIterator<TData>& ConstHashTableIterator<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++m_iterator;
            FindNextValidIterator();
        }
        return *this;
    }

    template<typename TData>
    constexpr ConstHashTableIterator<TData> ConstHashTableIterator<TData>::operator+(size_t offset)
    {
        ConstHashTableIterator iterator = *this;
        return iterator += offset;
    }

    template<typename TData>
    constexpr bool ConstHashTableIterator<TData>::operator==(const ConstHashTableIterator& other) const
    {
        return m_pBuckets == other.m_pBuckets &&
               m_index    == other.m_index    &&
               m_iterator == other.m_iterator;
    }

    template<typename TData>
    constexpr bool ConstHashTableIterator<TData>::HasReachedEnd() const
    {
        return m_index == m_pBuckets->Count();
    }

    template<typename TData>
    constexpr void ConstHashTableIterator<TData>::FindNextValidIterator()
    {
        while (!m_iterator.GetNode() && !HasReachedEnd())
        {
            ++m_index;

            if (HasReachedEnd())
            {
                m_iterator = ConstLinearNodeIterator<TData>(nullptr);
                return;
            }

            const TBucket& pBucket = m_pBuckets->At(m_index);
            m_iterator = pBucket.cbegin();
        }
    }
}