// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"

export module jpt.SortedSet;

import jpt.Allocator;
import jpt.Comparators;
import jpt.Concepts;
import jpt.Utilities;

import jpt_private.RedBlackTreeIterator;

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
		using TNode         = jpt_private::RedBlackTreeNode<TData>;
		using Iterator      = jpt_private::RedBlackTreeIterator<TData>;
		using ConstIterator = jpt_private::ConstRedBlackTreeIterator<TData>;

		using Color = typename TNode::EColor;

	public:
		static constexpr TComparator kComparator = TComparator();

	private:
		TNode* m_pRoot = nullptr;
		size_t m_count = 0;

	public:
		constexpr ~SortedSet();

		// Modifying
		constexpr void Add(const TData& data);
		constexpr void Clear();

		// Searching
		constexpr size_t Count() const;
		constexpr bool IsEmpty() const;

		// Iterators
		constexpr      Iterator begin();
		constexpr      Iterator end();
		constexpr ConstIterator begin()  const;
		constexpr ConstIterator end()    const;
		constexpr ConstIterator cbegin() const;
		constexpr ConstIterator cend()   const;

	private:
		// Traverse
		constexpr void PostOrderWalkNode(TNode* pNode, void(*pFunction)(TNode* pNode));

		// Searching
		constexpr       TNode* FindNode(const TData& data);
		constexpr const TNode* FindNode(const TData& data) const;
		constexpr       TNode* FindMinNode(TNode* pNode);
		constexpr const TNode* FindMinNode(TNode* pNode) const;

		// Red Black Tree Utils
		constexpr void FixAdd(TNode* pNode);
		constexpr void FixErase(TNode* pNode);
		constexpr void RotateLeft(TNode* pNode);
		constexpr void RotateRight(TNode* pNode);
		constexpr void Transplant(TNode* pOldNode, TNode* pNewNode);
	};

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr SortedSet<_TData, _Comparator, _TAllocator>::~SortedSet()
	{
		Clear();
	}

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr void SortedSet<_TData, _Comparator, _TAllocator>::Add(const TData& data)
	{
		if (TNode* pNode = FindNode(data); pNode != nullptr)
		{
			pNode->data = data;
			return;
		}

		TNode* pNewNode = TAllocator::AllocateWithValue(data);
		TNode* pParent = nullptr;
		TNode* pCurrent = m_pRoot;

		while (pCurrent != nullptr)
		{
			pParent = pCurrent;

			if (kComparator(data, pCurrent->data))
			{
				pCurrent = pCurrent->pLeftChild;
			}
			else
			{
				pCurrent = pCurrent->pRightChild;
			}
		}

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

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr void SortedSet<_TData, _Comparator, _TAllocator>::Clear()
	{
		PostOrderWalkNode(m_pRoot, [](TNode* pNode) 
			{
				TAllocator::Deallocate(pNode);
				pNode = nullptr;
			});
		m_pRoot = nullptr;
		m_count = 0;
	}

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr size_t SortedSet<_TData, _Comparator, _TAllocator>::Count() const
	{
		return m_count;
	}

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr bool SortedSet<_TData, _Comparator, _TAllocator>::IsEmpty() const
	{
		return m_count == 0;
	}

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr SortedSet<_TData, _Comparator, _TAllocator>::Iterator SortedSet<_TData, _Comparator, _TAllocator>::begin()
	{
		return Iterator(FindMinNode(m_pRoot));
	}

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr SortedSet<_TData, _Comparator, _TAllocator>::Iterator SortedSet<_TData, _Comparator, _TAllocator>::end()
	{
		return Iterator(nullptr);
	}

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr SortedSet<_TData, _Comparator, _TAllocator>::ConstIterator SortedSet<_TData, _Comparator, _TAllocator>::begin() const
	{
		return ConstIterator(FindMinNode(m_pRoot));
	}

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr SortedSet<_TData, _Comparator, _TAllocator>::ConstIterator SortedSet<_TData, _Comparator, _TAllocator>::end() const
	{
		return ConstIterator(nullptr);
	}

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr SortedSet<_TData, _Comparator, _TAllocator>::ConstIterator SortedSet<_TData, _Comparator, _TAllocator>::cbegin() const
	{
		return ConstIterator(FindMinNode(m_pRoot));
	}

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr SortedSet<_TData, _Comparator, _TAllocator>::ConstIterator SortedSet<_TData, _Comparator, _TAllocator>::cend() const
	{
		return ConstIterator(nullptr);
	}

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr void SortedSet<_TData, _Comparator, _TAllocator>::PostOrderWalkNode(TNode* pNode, void(*pFunction)(TNode* pNode))
	{
		if (pNode)
		{
			PostOrderWalkNode(pNode->pLeftChild, pFunction);
			PostOrderWalkNode(pNode->pRightChild, pFunction);
			pFunction(pNode);
		}
	}

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr SortedSet<_TData, _Comparator, _TAllocator>::TNode* SortedSet<_TData, _Comparator, _TAllocator>::FindNode(const TData& data)
	{
		TNode* pCurrent = m_pRoot;

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

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr const SortedSet<_TData, _Comparator, _TAllocator>::TNode* SortedSet<_TData, _Comparator, _TAllocator>::FindNode(const TData& data) const
	{
		return const_cast<SortedSet*>(this)->FindNode(data);
	}

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr SortedSet<_TData, _Comparator, _TAllocator>::TNode* SortedSet<_TData, _Comparator, _TAllocator>::FindMinNode(TNode* pNode)
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

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr const SortedSet<_TData, _Comparator, _TAllocator>::TNode* SortedSet<_TData, _Comparator, _TAllocator>::FindMinNode(TNode* pNode) const
	{
		return const_cast<SortedSet*>(this)->FindMinNode(pNode);
	}

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr void SortedSet<_TData, _Comparator, _TAllocator>::FixAdd(TNode* pNode)
	{
		while (pNode->pParent != nullptr && pNode->pParent->color == Color::Red)
		{
			if (pNode->pParent == pNode->pParent->pParent->pLeftChild)
			{
				TNode* pUncle = pNode->pParent->pParent->pRightChild;

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
				TNode* pUncle = pNode->pParent->pParent->pLeftChild;

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

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr void SortedSet<_TData, _Comparator, _TAllocator>::FixErase(TNode* pNode)
	{
		while (pNode != m_pRoot && pNode->color == Color::Black)
		{
			if (pNode == pNode->pParent->pLeftChild)
			{
				TNode* pSibling = pNode->pParent->pRightChild;

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
				TNode* pSibling = pNode->pParent->pLeftChild;

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

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr void SortedSet<_TData, _Comparator, _TAllocator>::RotateLeft(TNode* pNode)
	{
		TNode* pRightChild = pNode->pRightChild;
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

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr void SortedSet<_TData, _Comparator, _TAllocator>::RotateRight(TNode* pNode)
	{
		TNode* pLeftChild = pNode->pLeftChild;
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

	template<Comparable _TData, typename _Comparator, typename _TAllocator>
	constexpr void SortedSet<_TData, _Comparator, _TAllocator>::Transplant(TNode* pOldNode, TNode* pNewNode)
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
}