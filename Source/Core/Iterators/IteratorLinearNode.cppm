// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.IteratorLinearNode;

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
    class IteratorLinearNode
    {
    private:
        LinearNode<TData>* m_pNode;

    public:
        constexpr IteratorLinearNode() = default;
        constexpr IteratorLinearNode(LinearNode<TData>* pNode) : m_pNode(pNode) {}

        constexpr IteratorLinearNode& operator++();
        constexpr IteratorLinearNode operator++(int32);

        constexpr IteratorLinearNode& operator+=(size_t offset);
        constexpr IteratorLinearNode operator+(size_t offset);

        constexpr IteratorLinearNode& operator--();
        constexpr IteratorLinearNode operator--(int32);

        constexpr IteratorLinearNode& operator-=(size_t offset);
        constexpr IteratorLinearNode operator-(size_t offset);

        constexpr       TData* operator->()       { return &m_pNode->data; }
        constexpr const TData* operator->() const { return &m_pNode->data; }
        constexpr       TData* operator&()        { return &m_pNode->data; }
        constexpr const TData* operator&()  const { return &m_pNode->data; }
        constexpr       TData& operator*()        { return m_pNode->data; }
        constexpr const TData& operator*()  const { return m_pNode->data; }

        constexpr       LinearNode<TData>* GetNode()       { return m_pNode; }
        constexpr const LinearNode<TData>* GetNode() const { return m_pNode; }

        constexpr bool operator==(const IteratorLinearNode& other) const { return m_pNode == other.m_pNode; }
    };

    template<typename TData>
    constexpr IteratorLinearNode<TData>& IteratorLinearNode<TData>::operator++()
    {
        m_pNode = m_pNode->pNext;
        return *this;
    }

    template<typename TData>
    constexpr IteratorLinearNode<TData> IteratorLinearNode<TData>::operator++(int32)
    {
        IteratorLinearNode iterator = *this;
        m_pNode = m_pNode->pNext;
        return iterator;
    }

    template<typename TData>
    constexpr IteratorLinearNode<TData>& IteratorLinearNode<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            m_pNode = m_pNode->pNext;
        }

        return *this;
    }

    template<typename TData>
    constexpr IteratorLinearNode<TData> IteratorLinearNode<TData>::operator+(size_t offset)
    {
        IteratorLinearNode iterator = *this;
        return iterator += offset;
    }

    template<typename TData>
    constexpr IteratorLinearNode<TData>& IteratorLinearNode<TData>::operator--()
    {
        m_pNode = m_pNode->pPrevious;
        return *this;
    }

    template<typename TData>
    constexpr IteratorLinearNode<TData> IteratorLinearNode<TData>::operator--(int32)
    {
        IteratorLinearNode iterator = *this;
        m_pNode = m_pNode->pPrevious;
        return iterator;
    }

    template<typename TData>
    constexpr IteratorLinearNode<TData>& IteratorLinearNode<TData>::operator-=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            m_pNode = m_pNode->pPrevious;
        }

        return *this;
    }

    template<typename TData>
    constexpr IteratorLinearNode<TData> IteratorLinearNode<TData>::operator-(size_t offset)
    {
        IteratorLinearNode iterator = *this;
        return iterator -= offset;
    }

    template<typename TData>
    class ConstIteratorLinearNode
    {
    private:
        const LinearNode<TData>* m_pNode;

    public:
        constexpr ConstIteratorLinearNode() = default;
        constexpr ConstIteratorLinearNode(const LinearNode<TData>* pNode) : m_pNode(pNode) {}

        constexpr ConstIteratorLinearNode& operator++();
        constexpr ConstIteratorLinearNode operator++(int32);

        constexpr ConstIteratorLinearNode& operator+=(size_t offset);
        constexpr ConstIteratorLinearNode operator+(size_t offset);

        constexpr ConstIteratorLinearNode& operator--();
        constexpr ConstIteratorLinearNode operator--(int32);

        constexpr ConstIteratorLinearNode& operator-=(size_t offset);
        constexpr ConstIteratorLinearNode operator-(size_t offset);

        constexpr const TData* operator->() const { return &m_pNode->data; }
        constexpr const TData* operator&()  const { return &m_pNode->data; }
        constexpr const TData& operator*()  const { return  m_pNode->data; }

        constexpr const LinearNode<TData>* GetNode() const { return m_pNode; }

        constexpr bool operator==(const ConstIteratorLinearNode& other) const { return m_pNode == other.m_pNode; }
    };

    template<typename TData>
    constexpr ConstIteratorLinearNode<TData>& ConstIteratorLinearNode<TData>::operator++()
    {
        m_pNode = m_pNode->pNext;
        return *this;
    }

    template<typename TData>
    constexpr ConstIteratorLinearNode<TData> ConstIteratorLinearNode<TData>::operator++(int32)
    {
        ConstIteratorLinearNode iterator = *this;
        m_pNode = m_pNode->pNext;
        return iterator;
    }

    template<typename TData>
    constexpr ConstIteratorLinearNode<TData>& ConstIteratorLinearNode<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            m_pNode = m_pNode->pNext;
        }

        return *this;
    }

    template<typename TData>
    constexpr ConstIteratorLinearNode<TData> ConstIteratorLinearNode<TData>::operator+(size_t offset)
    {
        ConstIteratorLinearNode iterator = *this;
        return iterator += offset;
    }

    template<typename TData>
    constexpr ConstIteratorLinearNode<TData>& ConstIteratorLinearNode<TData>::operator--()
    {
        m_pNode = m_pNode->pPrevious;
        return *this;
    }

    template<typename TData>
    constexpr ConstIteratorLinearNode<TData> ConstIteratorLinearNode<TData>::operator--(int32)
    {
        ConstIteratorLinearNode iterator = *this;
        m_pNode = m_pNode->pPrevious;
        return iterator;
    }

    template<typename TData>
    constexpr ConstIteratorLinearNode<TData>& ConstIteratorLinearNode<TData>::operator-=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            m_pNode = m_pNode->pPrevious;
        }

        return *this;
    }

    template<typename TData>
    constexpr ConstIteratorLinearNode<TData> ConstIteratorLinearNode<TData>::operator-(size_t offset)
    {
        ConstIteratorLinearNode iterator = *this;
        return iterator -= offset;
    }
}