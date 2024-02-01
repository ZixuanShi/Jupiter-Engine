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
		while (m_pHead)
		{
			Node* pToDelete = m_pHead;
			m_pHead = m_pHead->pNext;
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
	template<typename ...TArgs>
	constexpr LinkedList<_TData>::TData& LinkedList<_TData>::EmplaceAfter(Iterator iterator, TArgs&& ...args)
	{
		Node* pNewNode = new Node(Forward<TArgs>(args)...);
		InternalInsertAfter(iterator, pNewNode);
		return pNewNode->data;
	}

	template<typename _TData>
	template<typename ...TArgs>
	constexpr LinkedList<_TData>::TData& LinkedList<_TData>::EmplaceBack(TArgs&& ...args)
	{
		return EmplaceAfter(m_pTail, Forward<TArgs>(args)...);
	}

	template<typename _TData>
	template<typename ...TArgs>
	constexpr LinkedList<_TData>::TData& LinkedList<_TData>::EmplaceBefore(Iterator iterator, TArgs&& ...args)
	{
		Node* pNewNode = new Node(Forward<TArgs>(args)...);
		InternalInsertBefore(iterator, pNewNode);
		return pNewNode->data;
	}

	template<typename _TData>
	template<typename ...TArgs>
	constexpr LinkedList<_TData>::TData& LinkedList<_TData>::EmplaceFront(TArgs&& ...args)
	{
		return EmplaceBefore(m_pHead, Forward<TArgs>(args)...);
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::Erase(Iterator iterator)
	{
		JPT_ASSERT(!IsEmpty(), "LinkedList::Erase() called on an empty list");

		Node* pToDelete = iterator.GetNode();
		JPT_ASSERT(pToDelete, "LinkedList::Erase() called with an invalid iterator");

		Node* pPrevious = pToDelete->pPrevious;
		Node* pNext = pToDelete->pNext;

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

		JPT_DELETE(pToDelete);
		--m_size;
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::PopBack()
	{
		Erase(m_pTail);
	}

	template<typename _TData>
	constexpr void LinkedList<_TData>::PopFront()
	{
		Erase(m_pHead);
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