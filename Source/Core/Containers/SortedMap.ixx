// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

export module jpt.SortedMap;

import jpt.Allocator;
import jpt.Comparators;
import jpt.Concepts;
import jpt.Constants;
import jpt.Function;
import jpt.Pair;
import jpt.Utilities;

import jpt_private.RedBlackTreeIterator;

export namespace jpt
{
	/** Sorted key value paired structure. Red Black Tree implementation */
	template<Comparable _TKey, typename _TValue, typename _TComparator = Comparator_Less<_TKey>, typename _TAllocator = Allocator<jpt_private::RedBlackTreeNode<Pair<const _TKey, _TValue>>>>
	class SortedMap
	{
	public:
		using TKey        = _TKey;
		using TValue      = _TValue;
		using TComparator = _TComparator;
		using TData       = Pair<const TKey, TValue>;
		using TNode       = jpt_private::RedBlackTreeNode<TData>;

		using Color 	  = typename TNode::EColor;
		using WalkerFunc  = Function<void(const TKey&, TValue&)>;

	public:
		static constexpr TComparator kComparator = TComparator();

	private:
		TNode* m_pRoot = nullptr;
		size_t m_count = 0;

	public:
		constexpr SortedMap() = default;
		constexpr ~SortedMap();

		// Modifying
		constexpr void Add(const TKey& key, const TValue& value);
		constexpr void Erase(const TKey& key);
		constexpr void Clear();

		// Searching
		constexpr size_t Count() const { return m_count; }
		constexpr bool Contains(const TKey& key);
		constexpr       TValue& operator[](const TKey& key);
		constexpr const TValue& operator[](const TKey& key) const;
		constexpr       TValue& Min();
		constexpr const TValue& Min() const;
		constexpr 	    TValue& Max();
		constexpr const TValue& Max() const;

		/** Root-left-right
			Used for prefixing. i.e. Creating a copy of the tree */
		constexpr void PreOrderWalk(const WalkerFunc& function);

		/** Left-root-right
			Used for sorting and finding the successor/predecessor of a node */
		constexpr void InOrderWalk(const WalkerFunc& function);

		/** Left-right-root
			Used for postfixing. i.e. Deleting the tree */
		constexpr void PostOrderWalk(const WalkerFunc& function);

	private:
		// Traverse
		constexpr void PreOrderWalk(TNode* pNode, const WalkerFunc& function);
		constexpr void InOrderWalk(TNode* pNode, const WalkerFunc& function);
		constexpr void PostOrderWalk(TNode* pNode, const WalkerFunc& function);
		constexpr void PostOrderWalkNode(TNode* pNode, const Function<void(TNode*)>& func);

		// Search
		constexpr TNode* FindNode(const TKey& key);
		constexpr TNode* FindMinNode(TNode* pNode);
		constexpr TNode* FindMaxNode(TNode* pNode);
		constexpr TNode* FindSuccessor(TNode* pNode);
		constexpr TNode* FindPredecessor(TNode* pNode);

		// Utils
		constexpr void RotateLeft(TNode* pNode);
		constexpr void RotateRight(TNode* pNode);
		constexpr void AddFixup(TNode* pNode);
		constexpr void Transplant(TNode* pOldNode, TNode* pNewNode);
		constexpr void EraseFixup(TNode* pNode);
	};

	template<Comparable _TKey, typename _TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<_TKey, _TValue, TComparator, TAllocator>::~SortedMap()
	{
		Clear();
	}

	template<Comparable _TKey, typename _TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<_TKey, _TValue, TComparator, TAllocator>::Add(const TKey& key, const TValue& value)
	{
		TNode* pNewNode = TAllocator::AllocateWithValue(TData(key, value));
		TNode* pParent = nullptr;
		TNode* pCurrent = m_pRoot;

		while (pCurrent != nullptr)
		{
			pParent = pCurrent;

			if (kComparator(key, pCurrent->data.first))
			{
				pCurrent = pCurrent->pLeftChild;
			}
			else
			{
				pCurrent = pCurrent->pRightChild;
			}
		}

		pNewNode->pParent = pParent;

		if (pParent == nullptr)
		{
			m_pRoot = pNewNode;
		}
		else if (kComparator(key, pParent->data.first))
		{
			pParent->pLeftChild = pNewNode;
		}
		else
		{
			pParent->pRightChild = pNewNode;
		}

		pNewNode->color = Color::Red;
		AddFixup(pNewNode);
		++m_count;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::Erase(const TKey& key)
	{
		TNode* pNode = FindNode(key);
		if (pNode == nullptr)
		{
			return;
		}

		Color originalColor = pNode->color;
		TNode* pChild = nullptr;

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
		else
		{
			TNode* pSuccessor = FindMinNode(pNode->pRightChild);
			originalColor = pSuccessor->color;
			pChild = pSuccessor->pRightChild;

			if (pSuccessor->pParent == pNode)
			{
				if (pChild != nullptr)
				{
					pChild->pParent = pSuccessor;
				}
			}
			else
			{
				Transplant(pSuccessor, pSuccessor->pRightChild);
				pSuccessor->pRightChild = pNode->pRightChild;
				pSuccessor->pRightChild->pParent = pSuccessor;
			}

			Transplant(pNode, pSuccessor);
			pSuccessor->pLeftChild = pNode->pLeftChild;
			pSuccessor->pLeftChild->pParent = pSuccessor;
			pSuccessor->color = pNode->color;
		}

		if (originalColor == Color::Black)
		{
			EraseFixup(pChild);
		}

		TAllocator::Deallocate(pNode);
		pNode = nullptr;
		--m_count;
	}

	template<Comparable _TKey, typename _TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<_TKey, _TValue, TComparator, TAllocator>::Clear()
	{
		PostOrderWalkNode(m_pRoot, [](TNode* pNode)
		{
			TAllocator::Deallocate(pNode);
			pNode = nullptr;
		});

		m_pRoot = nullptr;
		m_count = 0;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr bool SortedMap<TKey, TValue, TComparator, TAllocator>::Contains(const TKey& key)
	{
		return FindNode(key) != nullptr;
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TValue& SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::operator[](const TKey& key)
	{
		TNode* pNode = FindNode(key);

		if (pNode == nullptr)
		{
			Add(key, TValue());
			pNode = FindNode(key);
		}

		return pNode->data.second;
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr const SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TValue& SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::operator[](const TKey& key) const
	{
		TNode* pNode = FindNode(key);
		JPT_ASSERT(pNode != nullptr, "Key not found");
		return pNode->data.second;
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TValue& SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::Min()
	{
		return FindMinNode(m_pRoot)->data.second;
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr const SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TValue& SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::Min() const
	{
		return FindMinNode(m_pRoot)->data.second;
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TValue& SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::Max()
	{
		return FindMaxNode(m_pRoot)->data.second;
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr const SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TValue& SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::Max() const
	{
		return FindMaxNode(m_pRoot)->data.second;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::PreOrderWalk(const WalkerFunc& function)
	{
		PreOrderWalk(m_pRoot, function);
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::InOrderWalk(const WalkerFunc& function)
	{
		InOrderWalk(m_pRoot, function);
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::PostOrderWalk(const WalkerFunc& function)
	{
		PostOrderWalk(m_pRoot, function);
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::PreOrderWalk(TNode* pNode, const WalkerFunc& function)
	{
		if (pNode)
		{
			function(pNode->data.first, pNode->data.second);
			PreOrderWalk(pNode->pLeftChild, function);
			PreOrderWalk(pNode->pRightChild, function);
		}
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::InOrderWalk(TNode* pNode, const WalkerFunc& function)
	{
		if (pNode)
		{
			InOrderWalk(pNode->pLeftChild, function);
			function(pNode->data.first, pNode->data.second);
			InOrderWalk(pNode->pRightChild, function);
		}
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::PostOrderWalk(TNode* pNode, const WalkerFunc& function)
	{
		if (pNode)
		{
			PostOrderWalk(pNode->pLeftChild, function);
			PostOrderWalk(pNode->pRightChild, function);
			function(pNode->data.first, pNode->data.second);
		}
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::PostOrderWalkNode(TNode* pNode, const Function<void(TNode*)>& func)
	{
		if (pNode)
		{
			PostOrderWalkNode(pNode->pLeftChild, func);
			PostOrderWalkNode(pNode->pRightChild, func);
			func(pNode);
		}
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::TNode* SortedMap<TKey, TValue, TComparator, TAllocator>::FindNode(const TKey& key)
	{
		TNode* pCurrent = m_pRoot;

		while (pCurrent != nullptr)
		{
			if (pCurrent->data.first == key)
			{
				return pCurrent;
			}

			if (kComparator(key, pCurrent->data.first))
			{
				pCurrent = pCurrent->pLeftChild;
			}
			else
			{
				pCurrent = pCurrent->pRightChild;
			}
		}

		return nullptr;
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TNode* SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::FindMinNode(TNode* pNode)
	{
		while (pNode->pLeftChild != nullptr)
		{
			pNode = pNode->pLeftChild;
		}

		return pNode;
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TNode* SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::FindMaxNode(TNode* pNode)
	{
		while (pNode->pRightChild != nullptr)
		{
			pNode = pNode->pRightChild;
		}

		return pNode;
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TNode* SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::FindSuccessor(TNode* pNode)
	{
		if (pNode->pRightChild != nullptr)
		{
			return FindMinNode(pNode->pRightChild);
		}

		TNode* pParent = pNode->pParent;

		while (pParent != nullptr && pNode == pParent->pRightChild)
		{
			pNode = pParent;
			pParent = pParent->pParent;
		}

		return pParent;
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TNode* SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::FindPredecessor(TNode* pNode)
	{
		if (pNode->pLeftChild != nullptr)
		{
			return FindMaxNode(pNode->pLeftChild);
		}

		TNode* pParent = pNode->pParent;

		while (pParent != nullptr && pNode == pParent->pLeftChild)
		{
			pNode = pParent;
			pParent = pParent->pParent;
		}

		return pParent;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::RotateLeft(TNode* pNode)
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

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::RotateRight(TNode* pNode)
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

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::AddFixup(TNode* pNode)
	{
		while (pNode->pParent != nullptr && pNode->pParent->color == Color::Red)
		{
			if (pNode->pParent == pNode->pParent->pParent->pLeftChild)
			{
				TNode* pUncle = pNode->pParent->pParent->pRightChild;

				if (pUncle != nullptr && pUncle->color == Color::Red)
				{
					pNode->pParent->color = Color::Black;
					pUncle->color = Color::Black;
					pNode->pParent->pParent->color = Color::Red;
					pNode = pNode->pParent->pParent;
				}
				else
				{
					if (pNode == pNode->pParent->pRightChild)
					{
						pNode = pNode->pParent;
						RotateLeft(pNode);
					}

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

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::Transplant(TNode* pOldNode, TNode* pNewNode)
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

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::EraseFixup(TNode* pNode)
	{
		while (pNode != m_pRoot && pNode->color == Color::Black)
		{
			if (pNode == pNode->pParent->pLeftChild)
			{
				TNode* pSibling = pNode->pParent->pRightChild;

				if (pSibling->color == Color::Red)
				{
					pSibling->color = Color::Black;
					pNode->pParent->color = Color::Red;
					RotateLeft(pNode->pParent);
					pSibling = pNode->pParent->pRightChild;
				}

				if (pSibling->pLeftChild->color == Color::Black && pSibling->pRightChild->color == Color::Black)
				{
					pSibling->color = Color::Red;
					pNode = pNode->pParent;
				}
				else
				{
					if (pSibling->pRightChild->color == Color::Black)
					{
						pSibling->pLeftChild->color = Color::Black;
						pSibling->color = Color::Red;
						RotateRight(pSibling);
						pSibling = pNode->pParent->pRightChild;
					}

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
}