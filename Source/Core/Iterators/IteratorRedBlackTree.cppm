// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.IteratorRedBlackTree;

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
    class IteratorRedBlackTree
    {
    private:
        RedBlackTreeNode<TData>* m_pNode;

    public:
        constexpr IteratorRedBlackTree() = default;
        constexpr IteratorRedBlackTree(RedBlackTreeNode<TData>* pNode) : m_pNode(pNode) {}

        constexpr IteratorRedBlackTree& operator++();
        constexpr IteratorRedBlackTree operator++(int32);

        constexpr IteratorRedBlackTree& operator+=(size_t offset);
        constexpr IteratorRedBlackTree operator+(size_t offset);

        constexpr IteratorRedBlackTree& operator--();
        constexpr IteratorRedBlackTree operator--(int32);

        constexpr       TData* operator->()       { return &m_pNode->data; }
        constexpr const TData* operator->() const { return &m_pNode->data; }
        constexpr       TData& operator*()        { return m_pNode->data; }
        constexpr const TData& operator*()  const { return m_pNode->data; }

        constexpr RedBlackTreeNode<TData>* GetNode() const { return m_pNode; }

        constexpr bool operator==(const IteratorRedBlackTree& other) const;

    private:
        constexpr void FindNextValidIterator();
        constexpr void FindPreviousValidIterator();
    };

    template<typename TData>
    constexpr IteratorRedBlackTree<TData>& IteratorRedBlackTree<TData>::operator++()
    {
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr IteratorRedBlackTree<TData> IteratorRedBlackTree<TData>::operator++(int32)
    {
        IteratorRedBlackTree iterator = *this;
        ++(*this);
        return iterator;
    }

    template<typename TData>
    constexpr IteratorRedBlackTree<TData>& IteratorRedBlackTree<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++(*this);
        }

        return *this;
    }

    template<typename TData>
    constexpr IteratorRedBlackTree<TData> IteratorRedBlackTree<TData>::operator+(size_t offset)
    {
        IteratorRedBlackTree iterator = *this;
        iterator += offset;
        return iterator;
    }

    template<typename TData>
    constexpr IteratorRedBlackTree<TData>& IteratorRedBlackTree<TData>::operator--()
    {
        FindPreviousValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr IteratorRedBlackTree<TData> IteratorRedBlackTree<TData>::operator--(int32)
    {
        IteratorRedBlackTree iterator = *this;
        --(*this);
        return iterator;
    }

    template<typename TData>
    constexpr bool IteratorRedBlackTree<TData>::operator==(const IteratorRedBlackTree& other) const
    {
        return m_pNode == other.m_pNode;
    }

    template<typename TData>
    constexpr void IteratorRedBlackTree<TData>::FindNextValidIterator()
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
    constexpr void IteratorRedBlackTree<TData>::FindPreviousValidIterator()
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
    class ConstIteratorRedBlackTree
    {
    private:
        const RedBlackTreeNode<TData>* m_pNode;

    public:
        constexpr ConstIteratorRedBlackTree() = default;
        constexpr ConstIteratorRedBlackTree(const RedBlackTreeNode<TData>* pNode) : m_pNode(pNode) {}

        constexpr ConstIteratorRedBlackTree& operator++();
        constexpr ConstIteratorRedBlackTree operator++(int32);

        constexpr ConstIteratorRedBlackTree& operator+=(size_t offset);
        constexpr ConstIteratorRedBlackTree operator+(size_t offset);

        constexpr ConstIteratorRedBlackTree& operator--();
        constexpr ConstIteratorRedBlackTree operator--(int32);

        constexpr const TData* operator->() const { return &m_pNode->data; }
        constexpr const TData& operator*()  const { return m_pNode->data; }

        constexpr const RedBlackTreeNode<TData>* GetNode() const { return m_pNode; }

        constexpr bool operator==(const ConstIteratorRedBlackTree& other) const;

    private:
        constexpr void FindNextValidIterator();
        constexpr void FindPreviousValidIterator();
    };

    template<typename TData>
    constexpr ConstIteratorRedBlackTree<TData>& ConstIteratorRedBlackTree<TData>::operator++()
    {
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr ConstIteratorRedBlackTree<TData> ConstIteratorRedBlackTree<TData>::operator++(int32)
    {
        ConstIteratorRedBlackTree iterator = *this;
        ++(*this);
        return iterator;
    }

    template<typename TData>
    constexpr ConstIteratorRedBlackTree<TData>& ConstIteratorRedBlackTree<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++(*this);
        }

        return *this;
    }

    template<typename TData>
    constexpr ConstIteratorRedBlackTree<TData> ConstIteratorRedBlackTree<TData>::operator+(size_t offset)
    {
        ConstIteratorRedBlackTree iterator = *this;
        iterator += offset;
        return iterator;
    }

    template<typename TData>
    constexpr ConstIteratorRedBlackTree<TData>& ConstIteratorRedBlackTree<TData>::operator--()
    {
        FindPreviousValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr ConstIteratorRedBlackTree<TData> ConstIteratorRedBlackTree<TData>::operator--(int32)
    {
        ConstIteratorRedBlackTree iterator = *this;
        --(*this);
        return iterator;
    }

    template<typename TData>
    constexpr bool ConstIteratorRedBlackTree<TData>::operator==(const ConstIteratorRedBlackTree& other) const
    {
        return m_pNode == other.m_pNode;
    }

    template<typename TData>
    constexpr void ConstIteratorRedBlackTree<TData>::FindNextValidIterator()
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
    constexpr void ConstIteratorRedBlackTree<TData>::FindPreviousValidIterator()
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

