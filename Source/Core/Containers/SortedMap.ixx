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
		using WalkerFunc  = Function<void(const TKey&, TValue&)>;

	public:
		static constexpr TComparator kComparator = TComparator();

	private:
		TNode* m_pRoot = nullptr;
		size_t m_count = 0;

	public:
		constexpr SortedMap() = default;
		constexpr ~SortedMap();

		constexpr void Add(const TKey& key, const TValue& value);
		constexpr void Erase(const TKey& key);
		constexpr void Clear();

		constexpr size_t Count() const { return m_count; }

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
		constexpr void ForEachNode(TNode* pNode, const Function<void(TNode*)>& func);
		constexpr void PreOrderWalk(TNode* pNode, const WalkerFunc& function);
		constexpr void InOrderWalk(TNode* pNode, const WalkerFunc& function);
		constexpr void PostOrderWalk(TNode* pNode, const WalkerFunc& function);

		// Search
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

		// If the root is null, then this is the first node
		if (m_pRoot == nullptr)
		{
			m_pRoot = pNewNode;
			m_count = 1;
			return;
		}

		TNode* pCurrent = m_pRoot;
		TNode* pParent  = nullptr;

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

		if (kComparator(key, pParent->data.first))
		{
			pParent->pLeftChild = pNewNode;
		}
		else
		{
			pParent->pRightChild = pNewNode;
		}

		pNewNode->pParent = pParent;

		++m_count;
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::Erase(const TKey& key)
	{

	}

	template<Comparable _TKey, typename _TValue, typename TComparator, typename TAllocator>
	constexpr void SortedMap<_TKey, _TValue, TComparator, TAllocator>::Clear()
	{
		ForEachNode(m_pRoot, [](TNode* pNode)
		{
			TAllocator::Deallocate(pNode);
			pNode = nullptr;
		});

		m_pRoot = nullptr;
		m_count = 0;
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::PreOrderWalk(const WalkerFunc& function)
	{
		PreOrderWalk(m_pRoot, function);
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::InOrderWalk(const WalkerFunc& function)
	{
		InOrderWalk(m_pRoot, function);
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::PostOrderWalk(const WalkerFunc& function)
	{
		PostOrderWalk(m_pRoot, function);
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::PreOrderWalk(TNode* pNode, const WalkerFunc& function)
	{
		if (pNode)
		{
			function(pNode->data.first, pNode->data.second);
			PreOrderWalk(pNode->pLeftChild, function);
			PreOrderWalk(pNode->pRightChild, function);
		}
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::InOrderWalk(TNode* pNode, const WalkerFunc& function)
	{
		if (pNode)
		{
			InOrderWalk(pNode->pLeftChild, function);
			function(pNode->data.first, pNode->data.second);
			InOrderWalk(pNode->pRightChild, function);
		}
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::PostOrderWalk(TNode* pNode, const WalkerFunc& function)
	{
		if (pNode)
		{
			PostOrderWalk(pNode->pLeftChild, function);
			PostOrderWalk(pNode->pRightChild, function);
			function(pNode->data.first, pNode->data.second);
		}
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::ForEachNode(TNode* pNode, const Function<void(TNode*)>& func)
	{
		if (pNode)
		{
			ForEachNode(pNode->pLeftChild, func);
			ForEachNode(pNode->pRightChild, func);
			func(pNode);
		}
	}
}