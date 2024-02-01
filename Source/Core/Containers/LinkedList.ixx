// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <initializer_list>

export module jpt.LinkedList;

import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;

import jpt_private.LinearNodeIterator;

export namespace jpt
{
	/** A container that supports constant time insertion and removal of elements from anywhere in the container */
	template<typename _TData>
	class LinkedList
	{
	public:
		using TData         = _TData;
		using Node          = jpt_private::LinearNode<TData>;
		using Iterator      = jpt_private::LinearNodeIterator<TData>;
		using ConstIterator = jpt_private::ConstLinearNodeIterator<TData>;

	private:
		Node* m_pHead = nullptr;
		Node* m_pTail = nullptr;
		size_t m_size = 0;

	public:
		constexpr LinkedList() = default;
		constexpr LinkedList(const LinkedList& other);
		constexpr ~LinkedList();

		// Iterators
		constexpr Iterator begin() { return  Iterator(m_pHead); }
		constexpr Iterator end()   { return  Iterator(nullptr); }
		constexpr ConstIterator begin()  const { return ConstIterator(m_pHead); }
		constexpr ConstIterator end()    const { return ConstIterator(nullptr); }
		constexpr ConstIterator cbegin() const { return ConstIterator(m_pHead); }
		constexpr ConstIterator cend()   const { return ConstIterator(nullptr); }

		// Capacity
		constexpr bool IsEmpty() const { return m_size == 0; }

		// Modifier
		constexpr void Clear();

		// Inserting
		constexpr void InsertAfter(const Iterator& iterator, const TData& value);
		constexpr void PushBack(const TData& value);

	private:

	};

	template<typename _TData>
	constexpr LinkedList<_TData>::LinkedList(const LinkedList& other)
	{
		for (const TData& element : other)
		{
			PushBack(element);
		}

		m_size = other.m_size;
	}

	template<typename _TData>
	constexpr LinkedList<_TData>::~LinkedList()
	{
		Clear();
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::Clear()
	{
		Node* pCurrent = m_pHead;

		while (pCurrent)
		{
			Node* pToDelete = pCurrent;
			pCurrent = pCurrent->pNext;
			JPT_DELETE(pToDelete);
		}

		m_pHead = nullptr;
		m_pTail = nullptr;
		m_size  = 0;
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::InsertAfter(const Iterator& iterator, const TData& value)
	{
		Node* pNewNode = new Node(value);

		if (IsEmpty())
		{
			JPT_ASSERT(iterator == begin() &&
				       iterator == end(),
					   "LinkedList::Insert() called with invalid iterator");

			m_pHead = pNewNode;
			m_pTail = pNewNode;
		}
		else
		{
			Node* pCurrentNode = iterator.GetNode();
			Node* pNext = pCurrentNode->pNext;

			pCurrentNode->pNext = pNewNode;

			pNewNode->pPrevious = pCurrentNode;
			pNewNode->pNext = pNext;

			if (pNext)
			{
				pNext->pPrevious = pNewNode;
			}

			if (pCurrentNode == m_pTail)
			{
				m_pTail = pNewNode;
			}
		}

		++m_size;
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::PushBack(const TData& value)
	{
		InsertAfter(m_pTail, value);
	}
}