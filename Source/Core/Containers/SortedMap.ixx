// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
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
		using TKey          = _TKey;
		using TValue        = _TValue;
		using TComparator   = _TComparator;
		using TData         = Pair<const TKey, TValue>;
		using TNode         = jpt_private::RedBlackTreeNode<TData>;
		using Iterator      = jpt_private::RedBlackTreeIterator<TData>;
		using ConstIterator = jpt_private::ConstRedBlackTreeIterator<TData>;

		using Color 	    = typename TNode::EColor;
		using WalkerFunc    = Function<void(const TKey&, TValue&)>;

	public:
		static constexpr TComparator kComparator = TComparator();

	private:
		TNode* m_pRoot = nullptr;
		size_t m_count = 0;

	public:
		constexpr SortedMap() = default;
		constexpr SortedMap(const SortedMap& other);
		constexpr SortedMap(SortedMap&& other);
		constexpr SortedMap& operator=(const SortedMap& other);
		constexpr SortedMap& operator=(SortedMap&& other);
		constexpr ~SortedMap();

		// Modifying
		constexpr void Add(const TKey& key, const TValue& value);
		constexpr void Add(const TData& data);
		constexpr void Erase(const TKey& key);
		constexpr void Erase(Iterator iterator);
		constexpr void Clear();

		// Searching
		constexpr size_t Count() const;
		constexpr      Iterator Find(const TKey& key);
		constexpr ConstIterator Find(const TKey& key) const;
		constexpr bool Has(const TKey& key);
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

		// Iterators
		constexpr      Iterator begin();
		constexpr      Iterator end();
		constexpr ConstIterator begin()  const;
		constexpr ConstIterator end()    const;
		constexpr ConstIterator cbegin() const;
		constexpr ConstIterator cend()   const;

	private:
		/** Traverse
			    A
			   / \
			  B   C
			 / \
			D   E       		*/
		// Root-Left-Right. A, B, D, E, C. Used for copying or serializing the tree
		constexpr void PreOrderWalk(TNode* pNode, const WalkerFunc& function);

		// Left-Root-Right. D, B, E, A, C. Used for sorting and finding the successor/predecessor of a node
		constexpr void InOrderWalk(TNode* pNode, const WalkerFunc& function);

		// Left-Right-Root. D, E, B, C, A. Used for deleting the tree
		constexpr void PostOrderWalk(TNode* pNode, const WalkerFunc& function);
		constexpr void PostOrderWalkNode(TNode* pNode, const Function<void(TNode*)>& func);

		// Search
		constexpr       TNode* FindNode(const TKey& key);
		constexpr const TNode* FindNode(const TKey& key) const;
		constexpr       TNode* FindMinNode(TNode* pNode);
		constexpr const TNode* FindMinNode(TNode* pNode) const;
		constexpr       TNode* FindMaxNode(TNode* pNode);
		constexpr const TNode* FindMaxNode(TNode* pNode) const;
		constexpr       TNode* FindSuccessor(TNode* pNode);
		constexpr const TNode* FindSuccessor(TNode* pNode) const;
		constexpr       TNode* FindPredecessor(TNode* pNode);
		constexpr const TNode* FindPredecessor(TNode* pNode) const;

		// Red Black Tree Utils
		constexpr void FixAdd(TNode* pNode);
		constexpr void FixErase(TNode* pNode);
		constexpr void RotateLeft(TNode* pNode);
		constexpr void RotateRight(TNode* pNode);
		constexpr void Transplant(TNode* pOldNode, TNode* pNewNode);

		// Copy and move
		constexpr void CopyMap(const SortedMap& other);
		constexpr void MoveMap(SortedMap&& other);
	};

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::SortedMap(const SortedMap& other)
	{
		CopyMap(other);
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::SortedMap(SortedMap&& other)
	{
		MoveMap(Move(other));
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>& SortedMap<TKey, TValue, TComparator, TAllocator>::operator=(const SortedMap& other)
	{
		if (this != &other)
		{
			Clear();
			CopyMap(other);
		}

		return *this;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>& SortedMap<TKey, TValue, TComparator, TAllocator>::operator=(SortedMap&& other)
	{
		if (this != &other)
		{
			Clear();
			MoveMap(Move(other));
		}

		return *this;
	}

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

		// Tree is empty
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
		FixAdd(pNewNode);
		++m_count;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::Add(const TData& data)
	{
		Add(data.first, data.second);
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::Erase(const TKey& key)
	{
		TNode* pNode = FindNode(key);
		JPT_ASSERT(pNode != nullptr, "Key not found");

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

		if (originalColor == Color::Black && pChild)
		{
			FixErase(pChild);
		}

		TAllocator::Deallocate(pNode);
		--m_count;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::Erase(Iterator iterator)
	{
		Erase(iterator->first);
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
	constexpr size_t SortedMap<TKey, TValue, TComparator, TAllocator>::Count() const
	{
		return m_count;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::Iterator SortedMap<TKey, TValue, TComparator, TAllocator>::Find(const TKey& key)
	{
		return Iterator(FindNode(key));
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::ConstIterator SortedMap<TKey, TValue, TComparator, TAllocator>::Find(const TKey& key) const
	{
		return ConstIterator(FindNode(key));
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr bool SortedMap<TKey, TValue, TComparator, TAllocator>::Has(const TKey& key)
	{
		return FindNode(key) != nullptr;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::TValue& SortedMap<TKey, TValue, TComparator, TAllocator>::operator[](const TKey& key)
	{
		TNode* pNode = FindNode(key);

		if (pNode == nullptr)
		{
			Add(key, TValue());
			pNode = FindNode(key);
		}

		return pNode->data.second;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr const SortedMap<TKey, TValue, TComparator, TAllocator>::TValue& SortedMap<TKey, TValue, TComparator, TAllocator>::operator[](const TKey& key) const
	{
		TNode* pNode = FindNode(key);
		JPT_ASSERT(pNode != nullptr, "Key not found");
		return pNode->data.second;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::TValue& SortedMap<TKey, TValue, TComparator, TAllocator>::Min()
	{
		return FindMinNode(m_pRoot)->data.second;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr const SortedMap<TKey, TValue, TComparator, TAllocator>::TValue& SortedMap<TKey, TValue, TComparator, TAllocator>::Min() const
	{
		return FindMinNode(m_pRoot)->data.second;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::TValue& SortedMap<TKey, TValue, TComparator, TAllocator>::Max()
	{
		return FindMaxNode(m_pRoot)->data.second;
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr const SortedMap<TKey, TValue, TComparator, TAllocator>::TValue& SortedMap<TKey, TValue, TComparator, TAllocator>::Max() const
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
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::Iterator SortedMap<TKey, TValue, TComparator, TAllocator>::begin()
	{
		return Iterator(FindMinNode(m_pRoot));
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::Iterator SortedMap<TKey, TValue, TComparator, TAllocator>::end()
	{
		return Iterator(nullptr);
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::ConstIterator SortedMap<TKey, TValue, TComparator, TAllocator>::begin() const
	{
		return ConstIterator(FindMinNode(m_pRoot));
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::ConstIterator SortedMap<TKey, TValue, TComparator, TAllocator>::end() const
	{
		return ConstIterator(nullptr);
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::ConstIterator SortedMap<TKey, TValue, TComparator, TAllocator>::cbegin() const
	{
		return ConstIterator(FindMinNode(m_pRoot));
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::ConstIterator SortedMap<TKey, TValue, TComparator, TAllocator>::cend() const
	{
		return ConstIterator(nullptr);
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

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr const SortedMap<TKey, TValue, TComparator, TAllocator>::TNode* SortedMap<TKey, TValue, TComparator, TAllocator>::FindNode(const TKey& key) const
	{
		return const_cast<SortedMap*>(this)->FindNode(key);
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::TNode* SortedMap<TKey, TValue, TComparator, TAllocator>::FindMinNode(TNode* pNode)
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

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr const SortedMap<TKey, TValue, TComparator, TAllocator>::TNode* SortedMap<TKey, TValue, TComparator, TAllocator>::FindMinNode(TNode* pNode) const
	{
		return const_cast<SortedMap*>(this)->FindMinNode(pNode);
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::TNode* SortedMap<TKey, TValue, TComparator, TAllocator>::FindMaxNode(TNode* pNode)
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

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr const SortedMap<TKey, TValue, TComparator, TAllocator>::TNode* SortedMap<TKey, TValue, TComparator, TAllocator>::FindMaxNode(TNode* pNode) const
	{
		return const_cast<SortedMap*>(this)->FindMaxNode(pNode);
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::TNode* SortedMap<TKey, TValue, TComparator, TAllocator>::FindSuccessor(TNode* pNode)
	{
		if (!pNode)
		{
			return nullptr;
		}

		if (pNode->pRightChild)
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

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr const SortedMap<TKey, TValue, TComparator, TAllocator>::TNode* SortedMap<TKey, TValue, TComparator, TAllocator>::FindSuccessor(TNode* pNode) const
	{
		return const_cast<SortedMap*>(this)->FindSuccessor(pNode);
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr SortedMap<TKey, TValue, TComparator, TAllocator>::TNode* SortedMap<TKey, TValue, TComparator, TAllocator>::FindPredecessor(TNode* pNode)
	{
		if (!pNode)
		{
			return nullptr;
		}

		if (pNode->pLeftChild)
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
	constexpr const SortedMap<TKey, TValue, TComparator, TAllocator>::TNode* SortedMap<TKey, TValue, TComparator, TAllocator>::FindPredecessor(TNode* pNode) const
	{
		return const_cast<SortedMap*>(this)->FindPredecessor(pNode);
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::FixAdd(TNode* pNode)
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

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::FixErase(TNode* pNode)
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
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::CopyMap(const SortedMap& other)
	{
		for (const auto& [key, value] : other)
		{
			Add(key, value);
		}
	}

	template<Comparable TKey, typename TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<TKey, TValue, TComparator, TAllocator>::MoveMap(SortedMap&& other)
	{
		m_pRoot = other.m_pRoot;
		m_count = other.m_count;

		other.m_pRoot = nullptr;
		other.m_count = 0;
	}
}