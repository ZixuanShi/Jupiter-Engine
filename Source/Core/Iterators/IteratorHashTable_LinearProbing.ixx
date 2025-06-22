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
        Entry<TKey, TValue>* m_pBuckets;
        Index m_index = 0;  /**< Current index in the hash table */

    public:
        constexpr IteratorHashTable_LinearProbing() = default;
        constexpr IteratorHashTable_LinearProbing(Entry<TKey, TValue>* pBuckets, Index index);

        //constexpr IteratorHashTable_LinearProbing& operator++();
        //constexpr IteratorHashTable_LinearProbing operator++(int32);

        constexpr       Entry<TKey, TValue>::TData* operator->()       { return &m_pBuckets[m_index].data; }
        constexpr const Entry<TKey, TValue>::TData* operator->() const { return &m_pBuckets[m_index].data; }
        constexpr       Entry<TKey, TValue>::TData& operator*()        { return  m_pBuckets[m_index].data; }
        constexpr const Entry<TKey, TValue>::TData& operator*()  const { return  m_pBuckets[m_index].data; }
    };

    template<typename TKey, typename TValue, Index kCapacity>
    class ConstIteratorHashTable_LinearProbing
    {
    private:
        const Entry<TKey, TValue>* m_pBuckets;
        Index m_index = 0;  /**< Current index in the hash table */

    public:
        constexpr ConstIteratorHashTable_LinearProbing() = default;
        constexpr ConstIteratorHashTable_LinearProbing(const Entry<TKey, TValue>* pBuckets, Index index);
    };

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr IteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::IteratorHashTable_LinearProbing(Entry<TKey, TValue>* pBuckets, Index index)
        : m_pBuckets(pBuckets)
        , m_index(index)
    {
    }

    template<typename TKey, typename TValue, Index kCapacity>
    constexpr ConstIteratorHashTable_LinearProbing<TKey, TValue, kCapacity>::ConstIteratorHashTable_LinearProbing(const Entry<TKey, TValue>* pBuckets, Index index)
        : m_pBuckets(pBuckets)
        , m_index(index)
    {
    }
}