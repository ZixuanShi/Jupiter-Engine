// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

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
	/** A container that supports constant time insertion and removal of elements from anywhere in the container */
	template<typename _TData, typename _TAllocator = Allocator<jpt_private::LinearNode<_TData>>>
	class LinkedList
	{
	public:
		using TData         = _TData;
		using TAllocator    = _TAllocator;
		using TNode          = jpt_private::LinearNode<TData>;
		using Iterator      = jpt_private::LinearNodeIterator<TData>;
		using ConstIterator = jpt_private::ConstLinearNodeIterator<TData>;

	private:
		TNode* m_pHead = nullptr;
		TNode* m_pTail = nullptr;
		size_t m_size  = 0;

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
		constexpr size_t Size()  const { return m_size; }
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

		constexpr void InternalInsertAfter(Iterator iterator, TNode* pNewNode);
		constexpr void InternalInsertBefore(Iterator iterator, TNode* pNewNode);
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
		m_size  = 0;
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::InsertBefore(Iterator iterator, const TData& value)
	{
		TNode* pNewNode = TAllocator::AllocateWithValue(value);
		InternalInsertBefore(iterator, pNewNode);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::InsertBefore(Iterator iterator, TData&& value)
	{
		TNode* pNewNode = TAllocator::AllocateWithValue(Move(value));
		InternalInsertBefore(iterator, pNewNode);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::PushFront(const TData& value)
	{
		InsertBefore(m_pHead, value);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::PushFront(TData&& value)
	{
		InsertBefore(m_pHead, Move(value));
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::InsertAfter(Iterator iterator, const TData& value)
	{
		TNode* pNewNode = TAllocator::AllocateWithValue(value);
		InternalInsertAfter(iterator, pNewNode);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::InsertAfter(Iterator iterator, TData&& value)
	{
		TNode* pNewNode = TAllocator::AllocateWithValue(Move(value));
		InternalInsertAfter(iterator, pNewNode);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::PushBack(const TData& value)
	{
		InsertAfter(m_pTail, value);
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::PushBack(TData&& value)
	{
		InsertAfter(m_pTail, Move(value));
	}

	template<typename TData, typename TAllocator>
	template<typename ...TArgs>
	constexpr LinkedList<TData, TAllocator>::TData& LinkedList<TData, TAllocator>::EmplaceAfter(Iterator iterator, TArgs&& ...args)
	{
		TNode* pNewNode = TAllocator::AllocateWithValue(Forward<TArgs>(args)...);
		InternalInsertAfter(iterator, pNewNode);
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
		InternalInsertBefore(iterator, pNewNode);
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
		--m_size;
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
			PushBack(element);
		}
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::MoveData(LinkedList<TData>&& other)
	{
		m_pHead = other.m_pHead;
		m_pTail = other.m_pTail;
		m_size  = other.m_size;

		other.m_pHead = nullptr;
		other.m_pTail = nullptr;
		other.m_size  = 0;
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::InternalInsertAfter(Iterator iterator, TNode* pNewNode)
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

		++m_size;
	}

	template<typename TData, typename TAllocator>
	constexpr void LinkedList<TData, TAllocator>::InternalInsertBefore(Iterator iterator, TNode* pNewNode)
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

		++m_size;
	}
}