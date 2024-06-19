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
import jpt.Stack;
import jpt.Queue;
import jpt.Pair;
import jpt.Utilities;
import jpt.Math;

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

		// Traverse
		constexpr void TraversePreOrder(TNode* pStart, const WalkerFunc& function);
		constexpr void TraversePreOrder(const WalkerFunc& function);
		constexpr void TraverseInOrder(TNode* pStart, const WalkerFunc& function);
		constexpr void TraverseInOrder(const WalkerFunc& function);
		constexpr void TraversePostOrder(TNode* pStart, const WalkerFunc& function);
		constexpr void TraversePostOrder(const WalkerFunc& function);

	private:
		constexpr void ForEachNode(TNode* pStart, const Function<void(TNode*)>& func);
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
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TraversePreOrder(TNode* pStart, const WalkerFunc& function)
	{
		if (pStart == nullptr)
		{
			return;
		}

		Stack<TNode*> stack;
		stack.Reserve(m_count);

		stack.Emplace(pStart);

		while (!stack.IsEmpty())
		{
			TNode* pCurrent = stack.Peek();
			stack.Pop();

			if (pCurrent->pRightChild)
			{
				stack.Emplace(pCurrent->pRightChild);
			}
			if (pCurrent->pLeftChild)
			{
				stack.Emplace(pCurrent->pLeftChild);
			}

			function(pCurrent->data.first, pCurrent->data.second);
		}
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TraversePreOrder(const WalkerFunc& function)
	{
		TraversePreOrder(m_pRoot, function);
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TraverseInOrder(TNode* pStart, const WalkerFunc& function)
	{
		if (pStart == nullptr)
		{
			return;
		}

		Stack<TNode*> stack;
		stack.Reserve(m_count);

		TNode* pCurrent = pStart;

		while (pCurrent != nullptr || !stack.IsEmpty())
		{
			while (pCurrent != nullptr)
			{
				stack.Emplace(pCurrent);
				pCurrent = pCurrent->pLeftChild;
			}

			pCurrent = stack.Peek();
			stack.Pop();

			function(pCurrent->data.first, pCurrent->data.second);

			pCurrent = pCurrent->pRightChild;
		}
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TraverseInOrder(const WalkerFunc& function)
	{
		TraverseInOrder(m_pRoot, function);
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TraversePostOrder(TNode* pStart, const WalkerFunc& function)
	{
		if (pStart == nullptr)
		{
			return;
		}

		Stack<TNode*> stack;
		stack.Reserve(m_count);

		TNode* pCurrent = pStart;
		TNode* pLastVisited = nullptr;

		while (pCurrent != nullptr || !stack.IsEmpty())
		{
			while (pCurrent != nullptr)
			{
				stack.Emplace(pCurrent);
				pCurrent = pCurrent->pLeftChild;
			}

			pCurrent = stack.Peek();

			if (pCurrent->pRightChild != nullptr && pCurrent->pRightChild != pLastVisited)
			{
				pCurrent = pCurrent->pRightChild;
			}
			else
			{
				function(pCurrent->data.first, pCurrent->data.second);
				pLastVisited = pCurrent;
				stack.Pop();
				pCurrent = nullptr;
			}
		}
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::TraversePostOrder(const WalkerFunc& function)
	{
		TraversePostOrder(m_pRoot, function);
	}

	template<Comparable _TKey, typename _TValue, typename _TComparator, typename _TAllocator>
	constexpr void SortedMap<_TKey, _TValue, _TComparator, _TAllocator>::ForEachNode(TNode* pStart, const Function<void(TNode*)>& func)
	{
		if (pStart == nullptr)
		{
			return;
		}

		Stack<TNode*> stack;
		stack.Reserve(m_count);

		stack.Emplace(pStart);

		while (!stack.IsEmpty())
		{
			TNode* pCurrent = stack.Peek();
			stack.Pop();

			if (pCurrent->pRightChild)
			{
				stack.Emplace(pCurrent->pRightChild);
			}
			if (pCurrent->pLeftChild)
			{
				stack.Emplace(pCurrent->pLeftChild);
			}

			func(pCurrent);
		}
	}
}