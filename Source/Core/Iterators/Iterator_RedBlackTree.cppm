// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.Iterator_RedBlackTree;

import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;

export namespace jpt_private
{
    template<typename TData>
    struct RedBlackTreeNode
    {
        enum class Color : uint8
        {
            Red,
            Black
        };

        TData data;
        RedBlackTreeNode* pParent = nullptr;
        RedBlackTreeNode* pLeftChild = nullptr;
        RedBlackTreeNode* pRightChild = nullptr;
        Color color = Color::Red;

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
        color = Color::Red;
    }

    template<typename TData>
    class Iterator_RedBlackTree
    {
    private:
        RedBlackTreeNode<TData>* m_pNode;

    public:
        constexpr Iterator_RedBlackTree() = default;
        constexpr Iterator_RedBlackTree(RedBlackTreeNode<TData>* pNode) : m_pNode(pNode) {}

        constexpr Iterator_RedBlackTree& operator++();
        constexpr Iterator_RedBlackTree operator++(int32);

        constexpr Iterator_RedBlackTree& operator+=(size_t offset);
        constexpr Iterator_RedBlackTree operator+(size_t offset);

        constexpr Iterator_RedBlackTree& operator--();
        constexpr Iterator_RedBlackTree operator--(int32);

        constexpr       TData* operator->()       { return &m_pNode->data; }
        constexpr const TData* operator->() const { return &m_pNode->data; }
        constexpr       TData& operator*()        { return m_pNode->data; }
        constexpr const TData& operator*()  const { return m_pNode->data; }

        constexpr RedBlackTreeNode<TData>* GetNode() const { return m_pNode; }

        constexpr bool operator==(const Iterator_RedBlackTree& other) const;

    private:
        constexpr void FindNextValidIterator();
        constexpr void FindPreviousValidIterator();
    };

    template<typename TData>
    constexpr Iterator_RedBlackTree<TData>& Iterator_RedBlackTree<TData>::operator++()
    {
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr Iterator_RedBlackTree<TData> Iterator_RedBlackTree<TData>::operator++(int32)
    {
        Iterator_RedBlackTree iterator = *this;
        ++(*this);
        return iterator;
    }

    template<typename TData>
    constexpr Iterator_RedBlackTree<TData>& Iterator_RedBlackTree<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++(*this);
        }

        return *this;
    }

    template<typename TData>
    constexpr Iterator_RedBlackTree<TData> Iterator_RedBlackTree<TData>::operator+(size_t offset)
    {
        Iterator_RedBlackTree iterator = *this;
        iterator += offset;
        return iterator;
    }

    template<typename TData>
    constexpr Iterator_RedBlackTree<TData>& Iterator_RedBlackTree<TData>::operator--()
    {
        FindPreviousValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr Iterator_RedBlackTree<TData> Iterator_RedBlackTree<TData>::operator--(int32)
    {
        Iterator_RedBlackTree iterator = *this;
        --(*this);
        return iterator;
    }

    template<typename TData>
    constexpr bool Iterator_RedBlackTree<TData>::operator==(const Iterator_RedBlackTree& other) const
    {
        return m_pNode == other.m_pNode;
    }

    template<typename TData>
    constexpr void Iterator_RedBlackTree<TData>::FindNextValidIterator()
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
    constexpr void Iterator_RedBlackTree<TData>::FindPreviousValidIterator()
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
    class ConstIterator_RedBlackTree
    {
    private:
        const RedBlackTreeNode<TData>* m_pNode;

    public:
        constexpr ConstIterator_RedBlackTree() = default;
        constexpr ConstIterator_RedBlackTree(const RedBlackTreeNode<TData>* pNode) : m_pNode(pNode) {}

        constexpr ConstIterator_RedBlackTree& operator++();
        constexpr ConstIterator_RedBlackTree operator++(int32);

        constexpr ConstIterator_RedBlackTree& operator+=(size_t offset);
        constexpr ConstIterator_RedBlackTree operator+(size_t offset);

        constexpr ConstIterator_RedBlackTree& operator--();
        constexpr ConstIterator_RedBlackTree operator--(int32);

        constexpr const TData* operator->() const { return &m_pNode->data; }
        constexpr const TData& operator*()  const { return m_pNode->data; }

        constexpr const RedBlackTreeNode<TData>* GetNode() const { return m_pNode; }

        constexpr bool operator==(const ConstIterator_RedBlackTree& other) const;

    private:
        constexpr void FindNextValidIterator();
        constexpr void FindPreviousValidIterator();
    };

    template<typename TData>
    constexpr ConstIterator_RedBlackTree<TData>& ConstIterator_RedBlackTree<TData>::operator++()
    {
        FindNextValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr ConstIterator_RedBlackTree<TData> ConstIterator_RedBlackTree<TData>::operator++(int32)
    {
        ConstIterator_RedBlackTree iterator = *this;
        ++(*this);
        return iterator;
    }

    template<typename TData>
    constexpr ConstIterator_RedBlackTree<TData>& ConstIterator_RedBlackTree<TData>::operator+=(size_t offset)
    {
        for (size_t i = 0; i < offset; ++i)
        {
            ++(*this);
        }

        return *this;
    }

    template<typename TData>
    constexpr ConstIterator_RedBlackTree<TData> ConstIterator_RedBlackTree<TData>::operator+(size_t offset)
    {
        ConstIterator_RedBlackTree iterator = *this;
        iterator += offset;
        return iterator;
    }

    template<typename TData>
    constexpr ConstIterator_RedBlackTree<TData>& ConstIterator_RedBlackTree<TData>::operator--()
    {
        FindPreviousValidIterator();
        return *this;
    }

    template<typename TData>
    constexpr ConstIterator_RedBlackTree<TData> ConstIterator_RedBlackTree<TData>::operator--(int32)
    {
        ConstIterator_RedBlackTree iterator = *this;
        --(*this);
        return iterator;
    }

    template<typename TData>
    constexpr bool ConstIterator_RedBlackTree<TData>::operator==(const ConstIterator_RedBlackTree& other) const
    {
        return m_pNode == other.m_pNode;
    }

    template<typename TData>
    constexpr void ConstIterator_RedBlackTree<TData>::FindNextValidIterator()
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
    constexpr void ConstIterator_RedBlackTree<TData>::FindPreviousValidIterator()
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

