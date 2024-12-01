// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

#include <initializer_list>

export module jpt.LinkedList;

import jpt.Allocator;
import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;

import jpt_private.LinearNodeIterator;

export namespace jpt
{
	/** A container that supports constant time adding and removal of elements from anywhere in the container */
	template<typename _TData, typename _TAllocator = Allocator<jpt_private::LinearNode<_TData>>>
	class LinkedList
	{
	public:
		using TData         = _TData;
		using TAllocator    = _TAllocator;
		using TNode         = jpt_private::LinearNode<TData>;
		using Iterator      = jpt_private::LinearNodeIterator<TData>;
		using ConstIterator = jpt_private::ConstLinearNodeIterator<TData>;

	private:
		TNode* m_pHead = nullptr;
		TNode* m_pTail = nullptr;
		size_t m_count = 0;

	public:
		constexpr LinkedList() = default;
		constexpr LinkedList(const std::initializer_list<TData>& list);
		constexpr LinkedList(const LinkedList& other);
		constexpr LinkedList(LinkedList&& other) noexcept;
		constexpr LinkedList& operator=(const LinkedList& other);
		constexpr LinkedList& operator=(LinkedList&& other) noexcept;
		constexpr ~LinkedList();

		// Element Access
		constexpr       TData& Front()       { return m_pHead->data; }
		constexpr const TData& Front() const { return m_pHead->data; }
		constexpr       TData& Back()        { return m_pTail->data; }
		constexpr const TData& Back()  const { return m_pTail->data; }

		// Iterators
		constexpr Iterator begin() noexcept { return Iterator(m_pHead); }
		constexpr Iterator end()   noexcept { return Iterator(nullptr); }
		constexpr ConstIterator begin()  const noexcept { return ConstIterator(m_pHead); }
		constexpr ConstIterator end()    const noexcept { return ConstIterator(nullptr); }
		constexpr ConstIterator cbegin() const noexcept { return ConstIterator(m_pHead); }
		constexpr ConstIterator cend()   const noexcept { return ConstIterator(nullptr); }

		// Capacity
		constexpr size_t Count() const { return m_count; }
		constexpr bool IsEmpty() const { return m_count == 0; }

		// Modifier
		constexpr void Clear();
		constexpr void Reverse();

		// Adding
		constexpr void AddBefore(Iterator iterator, const TData& value);
		constexpr void AddBefore(Iterator iterator, TData&& value);
		constexpr void AddFront(const TData& value);
		constexpr void AddFront(TData&& value);

		constexpr void AddAfter(Iterator iterator, const TData& value);
		constexpr void AddAfter(Iterator iterator, TData&& value);
		constexpr void AddBack(const TData& value);
		constexpr void AddBack(TData&& value);

		template<typename ...TArgs>
		constexpr TData& EmplaceAfter(Iterator iterator, TArgs&&... args);
		template<typename ...TArgs>
		constexpr TData& EmplaceBack(TArgs&&... args);

		template<typename ...TArgs>
		constexpr TData& EmplaceBefore(Iterator iterator, TArgs&&... args);
		template<typename ...TArgs>
		constexpr TData& EmplaceFront(TArgs&&... args);

		// Erasing
		constexpr void Erase(Iterator iterator);
		constexpr void PopBack();
		constexpr void PopFront();

	private:
		template<Iterable TContainer>
		constexpr void CopyData(const TContainer& container);
		constexpr void MoveData(LinkedList<TData>&& other);

		constexpr void InternalAddAfter(Iterator iterator, TNode* pNewNode);
		constexpr void InternalAddBefore(Iterator iterator, TNode* pNewNode);
	};

	template<typename TData>
	constexpr bool operator==(const LinkedList<TData>& a, const LinkedList<TData>& b)
	{
		if (a.Count() != b.Count())
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

	template<typename TData, typename TAllocator>
	constexpr LinkedList<TData, TAllocator>::LinkedList(const std::initializer_list<TData>& list)
	{
		CopyData(list);
	}

	template<typename TData, typename TAllocator>
	constexpr LinkedList<TData, TAllocator>::LinkedList(const LinkedList& other)
	{
		CopyData(other);
	}

	template<typename TData, typename TAllocator>
	constexpr LinkedList<TData, TAllocator>::LinkedList(LinkedList&& other) noexcept
	{
		MoveData(Move(other));
	}

	template<typename TData, typename TAllocator>
	constexpr LinkedList<TData, TAllocator>& LinkedList<TData, TAllocator>::operator=(const LinkedList& other)
	{
		if (this != &other)
		{
			Clear();
			CopyData(other);
		}

		return *this;
	}

	template<typename TData, typename TAllocator>
	constexpr LinkedList<TData, TAllocator>& LinkedList<TData, TAllocator>::operator=(LinkedList&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			MoveData(Move(other));
		}

		return *this;
	}

	template<typename TData, typename TAllocator>
	constexpr LinkedList<TData, TAllocator>::~LinkedList()
	{
		Clear();
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::Clear()
	{
		while (m_pHead)
		{
			TNode* pToDelete = m_pHead;
			m_pHead = m_pHead->pNext;
			TAllocator::Deallocate(pToDelete);
		}

		m_pHead = nullptr;
		m_pTail = nullptr;
		m_count = 0;
	}

	template<typename _TData, typename _TAllocator>
	constexpr void LinkedList<_TData, _TAllocator>::Reverse()
	{
		TNode* pCurrent = m_pHead;

		while (pCurrent)
		{
			TNode* pOriginalPrevious = pCurrent->pPrevious;
			TNode* pOriginalNext = pCurrent->pNext;

			// Modify linkages
			pCurrent->pPrevious = pOriginalNext;
			pCurrent->pNext = pOriginalPrevious;

			// Move to the next node
			pCurrent = pOriginalNext;
		}

		Swap(m_pHead, m_pTail);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::AddBefore(Iterator iterator, const TData& value)
	{
		TNode* pNewNode = TAllocator::AllocateWithValue(value);
		InternalAddBefore(iterator, pNewNode);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::AddBefore(Iterator iterator, TData&& value)
	{
		TNode* pNewNode = TAllocator::AllocateWithValue(Move(value));
		InternalAddBefore(iterator, pNewNode);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::AddFront(const TData& value)
	{
		AddBefore(m_pHead, value);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::AddFront(TData&& value)
	{
		AddBefore(m_pHead, Move(value));
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::AddAfter(Iterator iterator, const TData& value)
	{
		TNode* pNewNode = TAllocator::AllocateWithValue(value);
		InternalAddAfter(iterator, pNewNode);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::AddAfter(Iterator iterator, TData&& value)
	{
		TNode* pNewNode = TAllocator::AllocateWithValue(Move(value));
		InternalAddAfter(iterator, pNewNode);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::AddBack(const TData& value)
	{
		AddAfter(m_pTail, value);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::AddBack(TData&& value)
	{
		AddAfter(m_pTail, Move(value));
	}

	template<typename TData, typename TAllocator>
	template<typename ...TArgs>
	constexpr LinkedList<TData, TAllocator>::TData& LinkedList<TData, TAllocator>::EmplaceAfter(Iterator iterator, TArgs&& ...args)
	{
		TNode* pNewNode = TAllocator::AllocateWithValue(Forward<TArgs>(args)...);
		InternalAddAfter(iterator, pNewNode);
		return pNewNode->data;
	}

	template<typename TData, typename TAllocator>
	template<typename ...TArgs>
	constexpr LinkedList<TData, TAllocator>::TData& LinkedList<TData, TAllocator>::EmplaceBack(TArgs&& ...args)
	{
		return EmplaceAfter(m_pTail, Forward<TArgs>(args)...);
	}

	template<typename TData, typename TAllocator>
	template<typename ...TArgs>
	constexpr LinkedList<TData, TAllocator>::TData& LinkedList<TData, TAllocator>::EmplaceBefore(Iterator iterator, TArgs&& ...args)
	{
		TNode* pNewNode = TAllocator::AllocateWithValue(Forward<TArgs>(args)...);
		InternalAddBefore(iterator, pNewNode);
		return pNewNode->data;
	}

	template<typename TData, typename TAllocator>
	template<typename ...TArgs>
	constexpr LinkedList<TData, TAllocator>::TData& LinkedList<TData, TAllocator>::EmplaceFront(TArgs&& ...args)
	{
		return EmplaceBefore(m_pHead, Forward<TArgs>(args)...);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::Erase(Iterator iterator)
	{
		JPT_ASSERT(!IsEmpty(), "LinkedList::Erase() called on an empty list");

		TNode* pToDelete = iterator.GetNode();
		JPT_ASSERT(pToDelete, "LinkedList::Erase() called with an invalid iterator");

		TNode* pPrevious = pToDelete->pPrevious;
		TNode* pNext = pToDelete->pNext;

		if (pToDelete == m_pHead)
		{
			m_pHead = pNext;
		}
		if (pToDelete == m_pTail)
		{
			m_pTail = pPrevious;
		}
		if (pPrevious)
		{
			pPrevious->pNext = pNext;
		}
		if (pNext)
		{
			pNext->pPrevious = pPrevious;
		}

		TAllocator::Deallocate(pToDelete);
		--m_count;
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::PopBack()
	{
		Erase(m_pTail);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::PopFront()
	{
		Erase(m_pHead);
	}

	template<typename TData, typename TAllocator>
	template<Iterable TContainer>
	constexpr void LinkedList<TData, TAllocator>::CopyData(const TContainer& container)
	{
		for (const TData& element : container)
		{
			AddBack(element);
		}
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::MoveData(LinkedList<TData>&& other)
	{
		m_pHead = other.m_pHead;
		m_pTail = other.m_pTail;
		m_count = other.m_count;

		other.m_pHead = nullptr;
		other.m_pTail = nullptr;
		other.m_count = 0;
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::InternalAddAfter(Iterator iterator, TNode* pNewNode)
	{
		TNode* pCurrentNode = iterator.GetNode();
		if (pCurrentNode)
		{
			TNode* pNext = pCurrentNode->pNext;

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

		++m_count;
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::InternalAddBefore(Iterator iterator, TNode* pNewNode)
	{
		TNode* pCurrentNode = iterator.GetNode();
		if (pCurrentNode)
		{
			TNode* pPrevious = pCurrentNode->pPrevious;

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

		++m_count;
	}
}