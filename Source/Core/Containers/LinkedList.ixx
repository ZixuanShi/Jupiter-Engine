// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <initializer_list>

export module jpt.LinkedList;

import jpt.Concepts;
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
		constexpr LinkedList(const std::initializer_list<TData>& list);
		constexpr LinkedList(const LinkedList& other);
		constexpr LinkedList(LinkedList&& other) noexcept;
		constexpr LinkedList& operator=(const LinkedList& other);
		constexpr LinkedList& operator=(LinkedList&& other) noexcept;
		constexpr ~LinkedList();

		// Iterators
		constexpr Iterator begin() { return  Iterator(m_pHead); }
		constexpr Iterator end()   { return  Iterator(nullptr); }
		constexpr ConstIterator begin()  const { return ConstIterator(m_pHead); }
		constexpr ConstIterator end()    const { return ConstIterator(nullptr); }
		constexpr ConstIterator cbegin() const { return ConstIterator(m_pHead); }
		constexpr ConstIterator cend()   const { return ConstIterator(nullptr); }

		// Capacity
		constexpr size_t Size() const { return m_size; }
		constexpr bool IsEmpty() const { return m_size == 0; }

		// Modifier
		constexpr void Clear();

		// Inserting
		constexpr void InsertBefore(Iterator iterator, const TData& value);
		constexpr void InsertBefore(Iterator iterator, TData&& value);
		constexpr void PushFront(const TData& value);
		constexpr void PushFront(TData&& value);

		constexpr void InsertAfter(Iterator iterator, const TData& value);
		constexpr void InsertAfter(Iterator iterator, TData&& value);
		constexpr void PushBack(const TData& value);
		constexpr void PushBack(TData&& value);

		// Erasing
		//constexpr void EraseBefore(Iterator iterator)
		//constexpr void EraseAfter(Iterator iterator)

	private:
		template<Iterable TContainer>
		constexpr void CopyData(const TContainer& container);
		constexpr void MoveData(LinkedList<TData>&& other);

		constexpr void InternalInsertAfter(Iterator iterator, Node* pNewNode);
		constexpr void InternalInsertBefore(Iterator iterator, Node* pNewNode);
	};

	template<typename TData>
	constexpr bool operator==(const LinkedList<TData>& a, const LinkedList<TData>& b)
	{
		if (a.Size() != b.Size())
		{
			return false;
		}
		
		for (auto aItr = a.cbegin(), bItr = b.cbegin(); 
			      aItr != a.cend();
			      ++aItr, ++bItr)
		{
			if (*aItr != *bItr)
			{
				return false;
			}
		}

		return true;
	}

	template<typename _TData>
	constexpr LinkedList<_TData>::LinkedList(const std::initializer_list<TData>& list)
	{
		CopyData(list);
	}

	template<typename _TData>
	constexpr LinkedList<_TData>::LinkedList(const LinkedList& other)
	{
		CopyData(other);
	}

	template<typename _TData>
	constexpr LinkedList<_TData>::LinkedList(LinkedList&& other) noexcept
	{
		MoveData(Move(other));
	}

	template<typename _TData>
	constexpr LinkedList<_TData>& LinkedList<_TData>::operator=(const LinkedList& other)
	{
		if (this != &other)
		{
			Clear();
			CopyData(other);
		}

		return *this;
	}

	template<typename _TData>
	constexpr LinkedList<_TData>& LinkedList<_TData>::operator=(LinkedList&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			MoveData(Move(other));
		}

		return *this;
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
	constexpr void LinkedList<_TData>::InsertBefore(Iterator iterator, const TData& value)
	{
		Node* pNewNode = new Node(value);
		InternalInsertBefore(iterator, pNewNode);
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::InsertBefore(Iterator iterator, TData&& value)
	{
		Node* pNewNode = new Node(Move(value));
		InternalInsertBefore(iterator, pNewNode);
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::PushFront(const TData& value)
	{
		InsertBefore(m_pHead, value);
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::PushFront(TData&& value)
	{
		InsertBefore(m_pHead, Move(value));
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::InsertAfter(Iterator iterator, const TData& value)
	{
		Node* pNewNode = new Node(value);
		InternalInsertAfter(iterator, pNewNode);
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::InsertAfter(Iterator iterator, TData&& value)
	{
		Node* pNewNode = new Node(Move(value));
		InternalInsertAfter(iterator, pNewNode);
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::PushBack(const TData& value)
	{
		InsertAfter(m_pTail, value);
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::PushBack(TData&& value)
	{
		InsertAfter(m_pTail, Move(value));
	}

	template<typename _TData>
	template<Iterable TContainer>
	constexpr void LinkedList<_TData>::CopyData(const TContainer& container)
	{
		for (const TData& element : container)
		{
			PushBack(element);
		}
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::MoveData(LinkedList<TData>&& other)
	{
		m_pHead = other.m_pHead;
		m_pTail = other.m_pTail;
		m_size  = other.m_size;

		other.m_pHead = nullptr;
		other.m_pTail = nullptr;
		other.m_size  = 0;
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::InternalInsertAfter(Iterator iterator, Node* pNewNode)
	{
		Node* pCurrentNode = iterator.GetNode();
		if (pCurrentNode)
		{
			Node* pNext = pCurrentNode->pNext;

			pCurrentNode->pNext = pNewNode;
			pNewNode->pPrevious = pCurrentNode;
			pNewNode->pNext = pNext;

			if (pNext)
			{
				pNext->pPrevious = pNewNode;
			}
		}

		if (!m_pHead)
		{
			m_pHead = pNewNode;
		}
		if (pCurrentNode == m_pTail)
		{
			m_pTail = pNewNode;
		}

		++m_size;
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::InternalInsertBefore(Iterator iterator, Node* pNewNode)
	{
		Node* pCurrentNode = iterator.GetNode();
		if (pCurrentNode)
		{
			Node* pPrevious = pCurrentNode->pPrevious;

			pCurrentNode->pPrevious = pNewNode;
			pNewNode->pNext = pCurrentNode;
			pNewNode->pPrevious = pPrevious;

			if (pPrevious)
			{
				pPrevious->pNext = pNewNode;
			}
		}

		if (!m_pTail)
		{
			m_pTail = pNewNode;
		}
		if (pCurrentNode == m_pHead)
		{
			m_pHead = pNewNode;
		}

		++m_size;
	}
}