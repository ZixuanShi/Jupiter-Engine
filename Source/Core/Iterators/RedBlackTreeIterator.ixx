// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.RedBlackTreeIterator;

import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;

export namespace jpt_private
{
    template<typename TData>
    struct RedBlackTreeNode
    {
        enum class EColor : uint8
        {
            Red,
            Black
        };

        TData data;
        RedBlackTreeNode* pParent = nullptr;
        RedBlackTreeNode* pLeftChild = nullptr;
        RedBlackTreeNode* pRightChild = nullptr;
        EColor color = EColor::Red;

        constexpr RedBlackTreeNode(const TData& _data) : data(_data) {}
        constexpr RedBlackTreeNode(TData&& _data) : data(jpt::Move(_data)) {}

        constexpr ~RedBlackTreeNode();
    };

    template<typename TData>
    constexpr RedBlackTreeNode<TData>::~RedBlackTreeNode()
    {
        if constexpr (!jpt::IsTriviallyDestructible<TData>)
        {
            data.~TData();
        }

        pParent = nullptr;
        pLeftChild = nullptr;
        pRightChild = nullptr;
        color = EColor::Red;
    }

    template<typename TData>
    class RedBlackTreeIterator
    {
    private:
        RedBlackTreeNode<TData>* m_pNode;

    public:
        constexpr RedBlackTreeIterator() = default;
        constexpr RedBlackTreeIterator(RedBlackTreeNode<TData>* pNode) : m_pNode(pNode) {}

        constexpr RedBlackTreeIterator& operator++();
        constexpr RedBlackTreeIterator operator++(int32);

        constexpr RedBlackTreeIterator& operator+=(size_t offset);
        constexpr RedBlackTreeIterator operator+(size_t offset);

        constexpr RedBlackTreeIterator& operator--();
        constexpr RedBlackTreeIterator operator--(int32);

        constexpr       TData* operator->()       { return &m_pNode->data; }
        constexpr const TData* operator->() const { return &m_pNode->data; }
        constexpr       TData& operator*()        { return m_pNode->data; }
        constexpr const TData& operator*()  const { return m_pNode->data; }

        constexpr RedBlackTreeNode<TData>* GetNode() const { return m_pNode; }

        constexpr bool operator==(const RedBlackTreeIterator& other) const;

    private:
        constexpr void FindNextValidIterator();
        constexpr void FindPreviousValidIterator();
    };

    template<typename TData>
    constexpr RedBlackTreeIterator<TData>& RedBlackTreeIterator<TData>::operator++()
    {
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr RedBlackTreeIterator<TData> RedBlackTreeIterator<TData>::operator++(int32)
    {
        RedBlackTreeIterator iterator = *this;
        ++(*this);
        return iterator;
    }

    template<typename TData>
    constexpr RedBlackTreeIterator<TData>& RedBlackTreeIterator<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++(*this);
        }

        return *this;
    }

    template<typename TData>
    constexpr RedBlackTreeIterator<TData> RedBlackTreeIterator<TData>::operator+(size_t offset)
    {
        RedBlackTreeIterator iterator = *this;
        iterator += offset;
        return iterator;
    }

    template<typename TData>
    constexpr RedBlackTreeIterator<TData>& RedBlackTreeIterator<TData>::operator--()
    {
        FindPreviousValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr RedBlackTreeIterator<TData> RedBlackTreeIterator<TData>::operator--(int32)
    {
        RedBlackTreeIterator iterator = *this;
        --(*this);
        return iterator;
    }

    template<typename TData>
    constexpr bool RedBlackTreeIterator<TData>::operator==(const RedBlackTreeIterator& other) const
    {
        return m_pNode == other.m_pNode;
    }

    template<typename TData>
    constexpr void RedBlackTreeIterator<TData>::FindNextValidIterator()
    {
        if (m_pNode->pRightChild)
        {
            m_pNode = m_pNode->pRightChild;
            while (m_pNode->pLeftChild)
            {
                m_pNode = m_pNode->pLeftChild;
            }
        }
        else
        {
            RedBlackTreeNode<TData>* pParent = m_pNode->pParent;
            while (pParent && m_pNode == pParent->pRightChild)
            {
                m_pNode = pParent;
                pParent = pParent->pParent;
            }
            m_pNode = pParent;
        }
    }

    template<typename TData>
    constexpr void RedBlackTreeIterator<TData>::FindPreviousValidIterator()
    {
        if (m_pNode->pLeftChild)
        {
            m_pNode = m_pNode->pLeftChild;
            while (m_pNode->pRightChild)
            {
                m_pNode = m_pNode->pRightChild;
            }
        }
        else
        {
            RedBlackTreeNode<TData>* pParent = m_pNode->pParent;
            while (pParent && m_pNode == pParent->pLeftChild)
            {
                m_pNode = pParent;
                pParent = pParent->pParent;
            }
            m_pNode = pParent;
        }
    }

    template<typename TData>
    class ConstRedBlackTreeIterator
    {
    private:
        const RedBlackTreeNode<TData>* m_pNode;

    public:
        constexpr ConstRedBlackTreeIterator() = default;
        constexpr ConstRedBlackTreeIterator(const RedBlackTreeNode<TData>* pNode) : m_pNode(pNode) {}

        constexpr ConstRedBlackTreeIterator& operator++();
        constexpr ConstRedBlackTreeIterator operator++(int32);

        constexpr ConstRedBlackTreeIterator& operator+=(size_t offset);
        constexpr ConstRedBlackTreeIterator operator+(size_t offset);

        constexpr ConstRedBlackTreeIterator& operator--();
        constexpr ConstRedBlackTreeIterator operator--(int32);

        constexpr const TData* operator->() const { return &m_pNode->data; }
        constexpr const TData& operator*()  const { return m_pNode->data; }

        constexpr const RedBlackTreeNode<TData>* GetNode() const { return m_pNode; }

        constexpr bool operator==(const ConstRedBlackTreeIterator& other) const;

    private:
        constexpr void FindNextValidIterator();
        constexpr void FindPreviousValidIterator();
    };

    template<typename TData>
    constexpr ConstRedBlackTreeIterator<TData>& ConstRedBlackTreeIterator<TData>::operator++()
    {
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr ConstRedBlackTreeIterator<TData> ConstRedBlackTreeIterator<TData>::operator++(int32)
    {
        ConstRedBlackTreeIterator iterator = *this;
        ++(*this);
        return iterator;
    }

    template<typename TData>
    constexpr ConstRedBlackTreeIterator<TData>& ConstRedBlackTreeIterator<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++(*this);
        }

        return *this;
    }

    template<typename TData>
    constexpr ConstRedBlackTreeIterator<TData> ConstRedBlackTreeIterator<TData>::operator+(size_t offset)
    {
        ConstRedBlackTreeIterator iterator = *this;
        iterator += offset;
        return iterator;
    }

    template<typename TData>
    constexpr ConstRedBlackTreeIterator<TData>& ConstRedBlackTreeIterator<TData>::operator--()
    {
        FindPreviousValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr ConstRedBlackTreeIterator<TData> ConstRedBlackTreeIterator<TData>::operator--(int32)
    {
        ConstRedBlackTreeIterator iterator = *this;
        --(*this);
        return iterator;
    }

    template<typename TData>
    constexpr bool ConstRedBlackTreeIterator<TData>::operator==(const ConstRedBlackTreeIterator& other) const
    {
        return m_pNode == other.m_pNode;
    }

    template<typename TData>
    constexpr void ConstRedBlackTreeIterator<TData>::FindNextValidIterator()
    {
        if (m_pNode->pRightChild)
        {
            m_pNode = m_pNode->pRightChild;
            while (m_pNode->pLeftChild)
            {
                m_pNode = m_pNode->pLeftChild;
            }
        }
        else
        {
            const RedBlackTreeNode<TData>* pParent = m_pNode->pParent;
            while (pParent && m_pNode == pParent->pRightChild)
            {
                m_pNode = pParent;
                pParent = pParent->pParent;
            }
            m_pNode = pParent;
        }
    }

    template<typename TData>
    constexpr void ConstRedBlackTreeIterator<TData>::FindPreviousValidIterator()
    {
        if (m_pNode->pLeftChild)
        {
            m_pNode = m_pNode->pLeftChild;
            while (m_pNode->pRightChild)
            {
                m_pNode = m_pNode->pRightChild;
            }
        }
        else
        {
            const RedBlackTreeNode<TData>* pParent = m_pNode->pParent;
            while (pParent && m_pNode == pParent->pLeftChild)
            {
                m_pNode = pParent;
                pParent = pParent->pParent;
            }
            m_pNode = pParent;
        }
    }
}

