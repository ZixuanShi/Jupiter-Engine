// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.Iterator_LinearNode;

import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;

export namespace jpt_private
{
    /** Linear Node Based Data container */
    template<typename TData>
    struct LinearNode
    {
        TData data;
        LinearNode* pPrevious = nullptr;
        LinearNode* pNext     = nullptr;

        constexpr LinearNode(const TData& _data) : data(_data) {}
        constexpr LinearNode(TData&& _data) : data(jpt::Move(_data)) {}

        constexpr ~LinearNode();
    };

    template<typename TData>
    constexpr LinearNode<TData>::~LinearNode()
    {
        if constexpr (!jpt::IsTriviallyDestructible<TData>)
        {
            data.~TData();
        }

        pPrevious = nullptr;
        pNext = nullptr;
    }

    /** Iterator for linear node based data, i.e. LinkedList */
    template<typename TData>
    class Iterator_LinearNode
    {
    private:
        LinearNode<TData>* m_pNode;

    public:
        constexpr Iterator_LinearNode() = default;
        constexpr Iterator_LinearNode(LinearNode<TData>* pNode) : m_pNode(pNode) {}

        constexpr Iterator_LinearNode& operator++();
        constexpr Iterator_LinearNode operator++(int32);

        constexpr Iterator_LinearNode& operator+=(size_t offset);
        constexpr Iterator_LinearNode operator+(size_t offset);

        constexpr Iterator_LinearNode& operator--();
        constexpr Iterator_LinearNode operator--(int32);

        constexpr Iterator_LinearNode& operator-=(size_t offset);
        constexpr Iterator_LinearNode operator-(size_t offset);

        constexpr       TData* operator->()       { return &m_pNode->data; }
        constexpr const TData* operator->() const { return &m_pNode->data; }
        constexpr       TData* operator&()        { return &m_pNode->data; }
        constexpr const TData* operator&()  const { return &m_pNode->data; }
        constexpr       TData& operator*()        { return m_pNode->data; }
        constexpr const TData& operator*()  const { return m_pNode->data; }

        constexpr       LinearNode<TData>* GetNode()       { return m_pNode; }
        constexpr const LinearNode<TData>* GetNode() const { return m_pNode; }

        constexpr bool operator==(const Iterator_LinearNode& other) const { return m_pNode == other.m_pNode; }
    };

    template<typename TData>
    constexpr Iterator_LinearNode<TData>& Iterator_LinearNode<TData>::operator++()
    {
        m_pNode = m_pNode->pNext;
        return *this;
    }

    template<typename TData>
    constexpr Iterator_LinearNode<TData> Iterator_LinearNode<TData>::operator++(int32)
    {
        Iterator_LinearNode iterator = *this;
        m_pNode = m_pNode->pNext;
        return iterator;
    }

    template<typename TData>
    constexpr Iterator_LinearNode<TData>& Iterator_LinearNode<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            m_pNode = m_pNode->pNext;
        }

        return *this;
    }

    template<typename TData>
    constexpr Iterator_LinearNode<TData> Iterator_LinearNode<TData>::operator+(size_t offset)
    {
        Iterator_LinearNode iterator = *this;
        return iterator += offset;
    }

    template<typename TData>
    constexpr Iterator_LinearNode<TData>& Iterator_LinearNode<TData>::operator--()
    {
        m_pNode = m_pNode->pPrevious;
        return *this;
    }

    template<typename TData>
    constexpr Iterator_LinearNode<TData> Iterator_LinearNode<TData>::operator--(int32)
    {
        Iterator_LinearNode iterator = *this;
        m_pNode = m_pNode->pPrevious;
        return iterator;
    }

    template<typename TData>
    constexpr Iterator_LinearNode<TData>& Iterator_LinearNode<TData>::operator-=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            m_pNode = m_pNode->pPrevious;
        }

        return *this;
    }

    template<typename TData>
    constexpr Iterator_LinearNode<TData> Iterator_LinearNode<TData>::operator-(size_t offset)
    {
        Iterator_LinearNode iterator = *this;
        return iterator -= offset;
    }

    template<typename TData>
    class ConstIterator_LinearNode
    {
    private:
        const LinearNode<TData>* m_pNode;

    public:
        constexpr ConstIterator_LinearNode() = default;
        constexpr ConstIterator_LinearNode(const LinearNode<TData>* pNode) : m_pNode(pNode) {}

        constexpr ConstIterator_LinearNode& operator++();
        constexpr ConstIterator_LinearNode operator++(int32);

        constexpr ConstIterator_LinearNode& operator+=(size_t offset);
        constexpr ConstIterator_LinearNode operator+(size_t offset);

        constexpr ConstIterator_LinearNode& operator--();
        constexpr ConstIterator_LinearNode operator--(int32);

        constexpr ConstIterator_LinearNode& operator-=(size_t offset);
        constexpr ConstIterator_LinearNode operator-(size_t offset);

        constexpr const TData* operator->() const { return &m_pNode->data; }
        constexpr const TData* operator&()  const { return &m_pNode->data; }
        constexpr const TData& operator*()  const { return  m_pNode->data; }

        constexpr const LinearNode<TData>* GetNode() const { return m_pNode; }

        constexpr bool operator==(const ConstIterator_LinearNode& other) const { return m_pNode == other.m_pNode; }
    };

    template<typename TData>
    constexpr ConstIterator_LinearNode<TData>& ConstIterator_LinearNode<TData>::operator++()
    {
        m_pNode = m_pNode->pNext;
        return *this;
    }

    template<typename TData>
    constexpr ConstIterator_LinearNode<TData> ConstIterator_LinearNode<TData>::operator++(int32)
    {
        ConstIterator_LinearNode iterator = *this;
        m_pNode = m_pNode->pNext;
        return iterator;
    }

    template<typename TData>
    constexpr ConstIterator_LinearNode<TData>& ConstIterator_LinearNode<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            m_pNode = m_pNode->pNext;
        }

        return *this;
    }

    template<typename TData>
    constexpr ConstIterator_LinearNode<TData> ConstIterator_LinearNode<TData>::operator+(size_t offset)
    {
        ConstIterator_LinearNode iterator = *this;
        return iterator += offset;
    }

    template<typename TData>
    constexpr ConstIterator_LinearNode<TData>& ConstIterator_LinearNode<TData>::operator--()
    {
        m_pNode = m_pNode->pPrevious;
        return *this;
    }

    template<typename TData>
    constexpr ConstIterator_LinearNode<TData> ConstIterator_LinearNode<TData>::operator--(int32)
    {
        ConstIterator_LinearNode iterator = *this;
        m_pNode = m_pNode->pPrevious;
        return iterator;
    }

    template<typename TData>
    constexpr ConstIterator_LinearNode<TData>& ConstIterator_LinearNode<TData>::operator-=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            m_pNode = m_pNode->pPrevious;
        }

        return *this;
    }

    template<typename TData>
    constexpr ConstIterator_LinearNode<TData> ConstIterator_LinearNode<TData>::operator-(size_t offset)
    {
        ConstIterator_LinearNode iterator = *this;
        return iterator -= offset;
    }
}