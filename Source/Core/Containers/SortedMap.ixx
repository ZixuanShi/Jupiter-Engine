// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

export module jpt.SortedMap;

import jpt.Allocator;
import jpt.Concepts;
import jpt.Constants;
import jpt.DynamicArray;
import jpt.Pair;
import jpt.Utilities;
import jpt.Math;

import jpt_private.BinaryTreeIterator;

export namespace jpt
{
	/** Sorted key value paired structure. Red Black Tree under the hood */
	template<Comparable _TKey, typename _TValue, typename _TAllocator = Allocator<jpt_private::BinaryTreeNode<Pair<_TKey, _TValue>>>>
	class SortedMap
	{
	public:
		using TKey   = _TKey;
		using TValue = _TValue;
		using TData  = Pair<TKey, TValue>;
		using TNode  = jpt_private::BinaryTreeNode<TData>;

	private:
		TNode* m_pRoot = nullptr;
		size_t m_size  = 0;

	public:
		constexpr SortedMap() = default;
		constexpr ~SortedMap();

		constexpr void Add(const TKey& key, const TValue& value);

		// Modifiers
		constexpr void Clear();

	private:

	};

	template<Comparable _TKey, typename _TValue, typename TAllocator>
	constexpr SortedMap<_TKey, _TValue, TAllocator>::~SortedMap()
	{
		Clear();
	}

	template<Comparable _TKey, typename _TValue, typename TAllocator>
	constexpr void SortedMap<_TKey, _TValue, TAllocator>::Add(const TKey& key, const TValue& value)
	{
		TNode* pNewNode = TAllocator::AllocateWithValue(TData(key, value));

		// If the root is null, then this is the first node
		if (m_pRoot == nullptr)
		{
			m_pRoot = pNewNode;
			m_size = 1;
			return;
		}

		TNode* pCurrent = m_pRoot;
		TNode* pParent  = nullptr;

		while (pCurrent != nullptr)
		{
			pParent = pCurrent;

			if (key < pCurrent->data.first)
			{
				pCurrent = pCurrent->pLeftChild;
			}
			else
			{
				pCurrent = pCurrent->pRightChild;
			}
		}

		if (key < pParent->data.first)
		{
			pParent->pLeftChild = pNewNode;
		}
		else
		{
			pParent->pRightChild = pNewNode;
		}

		pNewNode->pParent = pParent;

		++m_size;
	}

	template<Comparable _TKey, typename _TValue, typename TAllocator>
	constexpr void SortedMap<_TKey, _TValue, TAllocator>::Clear()
	{
		if (m_pRoot != nullptr)
		{
			DynamicArray<TNode*> stack;
			stack.Reserve(m_size);
			stack.EmplaceBack(m_pRoot);

			while (!stack.IsEmpty())
			{
				TNode* pNode = stack.Back();
				stack.PopBack();

				if (pNode->pLeftChild)
				{
					stack.EmplaceBack(pNode->pLeftChild);
				}
				if (pNode->pRightChild)
				{
					stack.EmplaceBack(pNode->pRightChild);
				}

				TAllocator::Deallocate(pNode);
			}
		}

		m_size = 0;
	}
}