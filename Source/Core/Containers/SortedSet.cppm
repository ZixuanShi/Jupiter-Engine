// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

#include <initializer_list>

export module jpt.SortedSet;

import jpt.Allocator;
import jpt.Comparators;
import jpt.Concepts;
import jpt.Function;
import jpt.Utilities;

import jpt_private.IteratorRedBlackTree;

export namespace jpt
{
    /** Sorted unique values. Red Black Tree implementation */
    template<Comparable _TData, typename _Comparator = Comparator_Less<_TData>, typename _TAllocator = Allocator<jpt_private::RedBlackTreeNode<_TData>>>
    class SortedSet
    {
    public:
        using TData         = _TData;
        using TComparator   = _Comparator;
        using TAllocator    = _TAllocator;
        using Node          = jpt_private::RedBlackTreeNode<TData>;
        using Iterator      = jpt_private::IteratorRedBlackTree<TData>;
        using ConstIterator = jpt_private::ConstIteratorRedBlackTree<TData>;

        using Color = typename Node::EColor;
        using WalkerFunc = Function<void(const TData&)>;

    public:
        static constexpr TComparator kComparator = TComparator();

    private:
        Node* m_pRoot = nullptr;
        size_t m_count = 0;

    public:
        constexpr SortedSet() = default;
        constexpr SortedSet(const std::initializer_list<TData>& list);
        constexpr SortedSet(const SortedSet& other);
        constexpr SortedSet(SortedSet&& other) noexcept;
        constexpr SortedSet& operator=(const SortedSet& other);
        constexpr SortedSet& operator=(SortedSet&& other) noexcept;
        constexpr ~SortedSet();

    public:
        // Adding
        constexpr void Add(const TData& data);

        // Erasing
        constexpr void Erase(const TData& data);
        constexpr void Erase(Iterator iterator);
        constexpr void Clear();

        // Iterators
        constexpr      Iterator begin() noexcept;
        constexpr      Iterator end()   noexcept;
        constexpr ConstIterator begin()  const noexcept;
        constexpr ConstIterator end()    const noexcept;
        constexpr ConstIterator cbegin() const noexcept;
        constexpr ConstIterator cend()   const noexcept;

        // Capacity
        constexpr size_t Count() const noexcept;
        constexpr bool IsEmpty() const noexcept;

        // Traverse
        constexpr void PreOrderWalk(const WalkerFunc& func);
        constexpr void InOrderWalk(const WalkerFunc& func);
        constexpr void PostOrderWalk(const WalkerFunc& func);

    private:
        // Traverse
        constexpr void PreOrderWalk(Node* pNode, const WalkerFunc& func);
        constexpr void InOrderWalk(Node* pNode, const WalkerFunc& func);
        constexpr void PostOrderWalk(Node* pNode, const WalkerFunc& func);
        constexpr void PostOrderWalkNode(Node* pNode, const Function<void(Node*)>& func);

        // Searching
        constexpr       Node* FindNode(const TData& data);
        constexpr const Node* FindNode(const TData& data) const;
        constexpr       Node* FindMinNode(Node* pNode);
        constexpr const Node* FindMinNode(Node* pNode) const;
        constexpr       Node* FindMaxNode(Node* pNode);
        constexpr const Node* FindMaxNode(Node* pNode) const;

        // Red Black Tree Utils
        constexpr void FixAdd(Node* pNode);
        constexpr void FixErase(Node* pNode);
        constexpr void RotateLeft(Node* pNode);
        constexpr void RotateRight(Node* pNode);
        constexpr void Transplant(Node* pOldNode, Node* pNewNode);

        // Copy Move
        constexpr void CopySet(const SortedSet& other);
        constexpr void MoveSet(SortedSet&& other);
    };

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>::SortedSet(const std::initializer_list<TData>& list)
    {
        for (const TData& data : list)
        {
            Add(data);
        }
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>::SortedSet(const SortedSet& other)
    {
        CopySet(other);
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>::SortedSet(SortedSet&& other) noexcept
    {
        MoveSet(Move(other));
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>& SortedSet<TData, Comparator, TAllocator>::operator=(const SortedSet& other)
    {
        if (this != &other)
        {
            Clear();
            CopySet(other);
        }

        return *this;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>& SortedSet<TData, Comparator, TAllocator>::operator=(SortedSet&& other) noexcept
    {
        if (this != &other)
        {
            Clear();
            MoveSet(Move(other));
        }

        return *this;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>::~SortedSet()
    {
        Clear();
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::Add(const TData& data)
    {
        // Find the parent node
        Node* pParent = nullptr;
        Node* pCurrent = m_pRoot;
        while (pCurrent != nullptr)
        {
            pParent = pCurrent;

            // Ensure data is unique
            if (data == pCurrent->data)
            {
                return;
            }

            if (kComparator(data, pCurrent->data))
            {
                pCurrent = pCurrent->pLeftChild;
            }
            else
            {
                pCurrent = pCurrent->pRightChild;
            }
        }
        Node* pNewNode = TAllocator::New(data);
        pNewNode->pParent = pParent;

        // Tree is empty
        if (pParent == nullptr)
        {
            m_pRoot = pNewNode;
        }
        else if (kComparator(data, pParent->data))
        {
            pParent->pLeftChild = pNewNode;
        }
        else
        {
            pParent->pRightChild = pNewNode;
        }

        pNewNode->color = Color::Red;
        FixAdd(pNewNode);
        ++m_count;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::Erase(const TData& data)
    {
        Node* pNode = FindNode(data);
        JPT_ASSERT(pNode != nullptr, "Data not found");

        Color originalColor = pNode->color;
        Node* pChild = nullptr;

        // Node has no children
        if (pNode->pLeftChild == nullptr)
        {
            pChild = pNode->pRightChild;
            Transplant(pNode, pNode->pRightChild);
        }
        else if (pNode->pRightChild == nullptr)
        {
            pChild = pNode->pLeftChild;
            Transplant(pNode, pNode->pLeftChild);
        }
        // Node has two children
        else
        {
            Node* pMinNode = FindMinNode(pNode->pRightChild);
            originalColor = pMinNode->color;
            pChild = pMinNode->pRightChild;

            if (pMinNode->pParent == pNode)
            {
                if (pChild != nullptr)
                {
                    pChild->pParent = pMinNode;
                }
            }
            else
            {
                Transplant(pMinNode, pMinNode->pRightChild);
                pMinNode->pRightChild = pNode->pRightChild;
                pMinNode->pRightChild->pParent = pMinNode;
            }

            Transplant(pNode, pMinNode);
            pMinNode->pLeftChild = pNode->pLeftChild;
            pMinNode->pLeftChild->pParent = pMinNode;
            pMinNode->color = pNode->color;
        }

        if (originalColor == Color::Black)
        {
            FixErase(pChild);
        }

        TAllocator::Delete(pNode);
        pNode = nullptr;
        --m_count;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::Erase(Iterator iterator)
    {
        Erase(*iterator);
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::Clear()
    {
        PostOrderWalkNode(m_pRoot, [](Node* pNode) 
            {
                TAllocator::Delete(pNode);
                pNode = nullptr;
            });
        m_pRoot = nullptr;
        m_count = 0;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>::Iterator SortedSet<TData, Comparator, TAllocator>::begin() noexcept
    {
        return Iterator(FindMinNode(m_pRoot));
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>::Iterator SortedSet<TData, Comparator, TAllocator>::end() noexcept
    {
        return Iterator(nullptr);
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>::ConstIterator SortedSet<TData, Comparator, TAllocator>::begin() const noexcept
    {
        return ConstIterator(FindMinNode(m_pRoot));
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>::ConstIterator SortedSet<TData, Comparator, TAllocator>::end() const noexcept
    {
        return ConstIterator(nullptr);
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>::ConstIterator SortedSet<TData, Comparator, TAllocator>::cbegin() const noexcept
    {
        return ConstIterator(FindMinNode(m_pRoot));
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>::ConstIterator SortedSet<TData, Comparator, TAllocator>::cend() const noexcept
    {
        return ConstIterator(nullptr);
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr size_t SortedSet<TData, Comparator, TAllocator>::Count() const noexcept
    {
        return m_count;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr bool SortedSet<TData, Comparator, TAllocator>::IsEmpty() const noexcept
    {
        return m_count == 0;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::PreOrderWalk(const WalkerFunc& func)
    {
        PreOrderWalk(m_pRoot, func);
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::InOrderWalk(const WalkerFunc& func)
    {
        InOrderWalk(m_pRoot, func);
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::PostOrderWalk(const WalkerFunc& func)
    {
        PostOrderWalk(m_pRoot, func);
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::PreOrderWalk(Node* pNode, const WalkerFunc& func)
    {
        if (pNode)
        {
            func(pNode->data);
            PreOrderWalk(pNode->pLeftChild, func);
            PreOrderWalk(pNode->pRightChild, func);
        }
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::InOrderWalk(Node* pNode, const WalkerFunc& func)
    {
        if (pNode)
        {
            InOrderWalk(pNode->pLeftChild, func);
            func(pNode->data);
            InOrderWalk(pNode->pRightChild, func);
        }
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::PostOrderWalk(Node* pNode, const WalkerFunc& func)
    {
        if (pNode)
        {
            InOrderWalk(pNode->pLeftChild, func);
            InOrderWalk(pNode->pRightChild, func);
            func(pNode->data);
        }
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::PostOrderWalkNode(Node* pNode, const Function<void(Node*)>& func)
    {
        if (pNode)
        {
            PostOrderWalkNode(pNode->pLeftChild, func);
            PostOrderWalkNode(pNode->pRightChild, func);
            func(pNode);
        }
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>::Node* SortedSet<TData, Comparator, TAllocator>::FindNode(const TData& data)
    {
        Node* pCurrent = m_pRoot;

        while (pCurrent != nullptr)
        {
            if (kComparator(data, pCurrent->data))
            {
                pCurrent = pCurrent->pLeftChild;
            }
            else if (kComparator(pCurrent->data, data))
            {
                pCurrent = pCurrent->pRightChild;
            }
            else
            {
                return pCurrent;
            }
        }

        return nullptr;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr const SortedSet<TData, Comparator, TAllocator>::Node* SortedSet<TData, Comparator, TAllocator>::FindNode(const TData& data) const
    {
        return const_cast<SortedSet*>(this)->FindNode(data);
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>::Node* SortedSet<TData, Comparator, TAllocator>::FindMinNode(Node* pNode)
    {
        if (!pNode)
        {
            return nullptr;
        }

        while (pNode->pLeftChild)
        {
            pNode = pNode->pLeftChild;
        }

        return pNode;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr const SortedSet<TData, Comparator, TAllocator>::Node* SortedSet<TData, Comparator, TAllocator>::FindMinNode(Node* pNode) const
    {
        return const_cast<SortedSet*>(this)->FindMinNode(pNode);
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr SortedSet<TData, Comparator, TAllocator>::Node* SortedSet<TData, Comparator, TAllocator>::FindMaxNode(Node* pNode)
    {
        if (!pNode)
        {
            return nullptr;
        }

        while (pNode->pRightChild)
        {
            pNode = pNode->pRightChild;
        }

        return pNode;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr const SortedSet<TData, Comparator, TAllocator>::Node* SortedSet<TData, Comparator, TAllocator>::FindMaxNode(Node* pNode) const
    {
        return const_cast<SortedSet*>(this)->FindMaxNode(pNode);
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::FixAdd(Node* pNode)
    {
        while (pNode->pParent != nullptr && pNode->pParent->color == Color::Red)
        {
            if (pNode->pParent == pNode->pParent->pParent->pLeftChild)
            {
                Node* pUncle = pNode->pParent->pParent->pRightChild;

                // Case 1: Uncle is red. Recolor parent, uncle and grandparent
                if (pUncle != nullptr && pUncle->color == Color::Red)
                {
                    pNode->pParent->color = Color::Black;
                    pUncle->color = Color::Black;
                    pNode->pParent->pParent->color = Color::Red;
                    pNode = pNode->pParent->pParent;
                }
                else
                {
                    // Case 2: Uncle is black and pNode is right child. Rotate left
                    if (pNode == pNode->pParent->pRightChild)
                    {
                        pNode = pNode->pParent;
                        RotateLeft(pNode);
                    }

                    // Case 3: Uncle is black and pNode is left child. Recolor parent and grandparent and rotate right
                    pNode->pParent->color = Color::Black;
                    pNode->pParent->pParent->color = Color::Red;
                    RotateRight(pNode->pParent->pParent);
                }
            }
            else
            {
                Node* pUncle = pNode->pParent->pParent->pLeftChild;

                if (pUncle != nullptr && pUncle->color == Color::Red)
                {
                    pNode->pParent->color = Color::Black;
                    pUncle->color = Color::Black;
                    pNode->pParent->pParent->color = Color::Red;
                    pNode = pNode->pParent->pParent;
                }
                else
                {
                    if (pNode == pNode->pParent->pLeftChild)
                    {
                        pNode = pNode->pParent;
                        RotateRight(pNode);
                    }

                    pNode->pParent->color = Color::Black;
                    pNode->pParent->pParent->color = Color::Red;
                    RotateLeft(pNode->pParent->pParent);
                }
            }
        }

        m_pRoot->color = Color::Black;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::FixErase(Node* pNode)
    {
        while (pNode != m_pRoot && pNode->color == Color::Black)
        {
            if (pNode == pNode->pParent->pLeftChild)
            {
                Node* pSibling = pNode->pParent->pRightChild;

                // Case 1: Sibling is red. Recolor sibling and parent and rotate left
                if (pSibling->color == Color::Red)
                {
                    pSibling->color = Color::Black;
                    pNode->pParent->color = Color::Red;
                    RotateLeft(pNode->pParent);
                    pSibling = pNode->pParent->pRightChild;
                }

                // Case 2: Sibling is black and both children are black. Recolor sibling and move up the tree
                if (pSibling->pLeftChild->color == Color::Black && pSibling->pRightChild->color == Color::Black)
                {
                    pSibling->color = Color::Red;
                    pNode = pNode->pParent;
                }
                else
                {
                    // Case 3: Sibling is black and left child is red. Recolor sibling and left child and rotate right
                    if (pSibling->pRightChild->color == Color::Black)
                    {
                        pSibling->pLeftChild->color = Color::Black;
                        pSibling->color = Color::Red;
                        RotateRight(pSibling);
                        pSibling = pNode->pParent->pRightChild;
                    }

                    // Case 4: Sibling is black and right child is red. Recolor sibling and parent and rotate left
                    pSibling->color = pNode->pParent->color;
                    pNode->pParent->color = Color::Black;
                    pSibling->pRightChild->color = Color::Black;
                    RotateLeft(pNode->pParent);
                    pNode = m_pRoot;
                }
            }
            else
            {
                Node* pSibling = pNode->pParent->pLeftChild;

                if (pSibling->color == Color::Red)
                {
                    pSibling->color = Color::Black;
                    pNode->pParent->color = Color::Red;
                    RotateRight(pNode->pParent);
                    pSibling = pNode->pParent->pLeftChild;
                }

                if (pSibling->pRightChild->color == Color::Black && pSibling->pLeftChild->color == Color::Black)
                {
                    pSibling->color = Color::Red;
                    pNode = pNode->pParent;
                }
                else
                {
                    if (pSibling->pLeftChild->color == Color::Black)
                    {
                        pSibling->pRightChild->color = Color::Black;
                        pSibling->color = Color::Red;
                        RotateLeft(pSibling);
                        pSibling = pNode->pParent->pLeftChild;
                    }

                    pSibling->color = pNode->pParent->color;
                    pNode->pParent->color = Color::Black;
                    pSibling->pLeftChild->color = Color::Black;
                    RotateRight(pNode->pParent);
                    pNode = m_pRoot;
                }
            }
        }

        pNode->color = Color::Black;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::RotateLeft(Node* pNode)
    {
        Node* pRightChild = pNode->pRightChild;
        pNode->pRightChild = pRightChild->pLeftChild;

        if (pRightChild->pLeftChild != nullptr)
        {
            pRightChild->pLeftChild->pParent = pNode;
        }

        pRightChild->pParent = pNode->pParent;

        if (pNode->pParent == nullptr)
        {
            m_pRoot = pRightChild;
        }
        else if (pNode == pNode->pParent->pLeftChild)
        {
            pNode->pParent->pLeftChild = pRightChild;
        }
        else
        {
            pNode->pParent->pRightChild = pRightChild;
        }

        pRightChild->pLeftChild = pNode;
        pNode->pParent = pRightChild;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::RotateRight(Node* pNode)
    {
        Node* pLeftChild = pNode->pLeftChild;
        pNode->pLeftChild = pLeftChild->pRightChild;

        if (pLeftChild->pRightChild != nullptr)
        {
            pLeftChild->pRightChild->pParent = pNode;
        }

        pLeftChild->pParent = pNode->pParent;

        if (pNode->pParent == nullptr)
        {
            m_pRoot = pLeftChild;
        }
        else if (pNode == pNode->pParent->pRightChild)
        {
            pNode->pParent->pRightChild = pLeftChild;
        }
        else
        {
            pNode->pParent->pLeftChild = pLeftChild;
        }

        pLeftChild->pRightChild = pNode;
        pNode->pParent = pLeftChild;
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::Transplant(Node* pOldNode, Node* pNewNode)
    {
        if (pOldNode->pParent == nullptr)
        {
            m_pRoot = pNewNode;
        }
        else if (pOldNode == pOldNode->pParent->pLeftChild)
        {
            pOldNode->pParent->pLeftChild = pNewNode;
        }
        else
        {
            pOldNode->pParent->pRightChild = pNewNode;
        }

        if (pNewNode != nullptr)
        {
            pNewNode->pParent = pOldNode->pParent;
        }
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::CopySet(const SortedSet& other)
    {
        for (const TData& data : other)
        {
            Add(data);
        }
    }

    template<Comparable TData, typename Comparator, typename TAllocator>
    constexpr void SortedSet<TData, Comparator, TAllocator>::MoveSet(SortedSet&& other)
    {
        m_pRoot = other.m_pRoot;
        m_count = other.m_count;

        other.m_pRoot = nullptr;
        other.m_count = 0;
    }
}