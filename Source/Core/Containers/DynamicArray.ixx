// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <initializer_list>
#include <string>

export module jpt.DynamicArray;

import jpt.Allocator;
import jpt.ToString;
import jpt.Math;
import jpt_private.ContiguousIterator;

export namespace jpt
{
	/** A sequence container that encapsulates dynamic size arrays. */
	template<typename _TData, class _TAllocator = Allocator<_TData>>
	class DynamicArray
	{
	public:
		using TData      = _TData;
		using TAllocator = _TAllocator;

	private:
		TData* m_pBuffer  = nullptr;
		size_t m_size     = 0;
		size_t m_capacity = 0;

	public:
		constexpr DynamicArray() = default;
		constexpr DynamicArray(const std::initializer_list<TData>& list);
		constexpr ~DynamicArray();

		constexpr size_t Size() const { return m_size; }
		constexpr size_t size() const { return m_size; }	// Used for matching initializer list

	private:
		/** Create a new data buffer with a new capacity, move the existing data over */
		constexpr void UpdateBuffer(size_t capacity);
		
		/** Copy other's data through it's iterators. TOtherContainer should be guaranteed to provide iterator interfaces */
		template<Iterable TOtherContainer>
		constexpr void CopyIterators(const TOtherContainer& container, size_t size);
	};

	template<typename TData, class TAllocator>
	constexpr DynamicArray<TData, TAllocator>::DynamicArray(const std::initializer_list<TData>& list)
	{
		CopyIterators(list, list.size());
	}

	template<typename _TData, class _TAllocator>
	constexpr DynamicArray<_TData, _TAllocator>::~DynamicArray()
	{
		if constexpr (!IsTriviallyDestructible<TData>)
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				m_pBuffer[i].~TData();
			}
		}

		JPT_DELETE(m_pBuffer);
	}

	template<typename _TData, class _TAllocator>
	constexpr void DynamicArray<_TData, _TAllocator>::UpdateBuffer(size_t capacity)
	{
		static constexpr size_t kMinCapacity = 4;
		capacity = Max(capacity, kMinCapacity);

		TData* pNewBuffer = TAllocator::AllocateArray(capacity);

		if (m_pBuffer)
		{
			if constexpr (IsTriviallyCopyable<TData>)
			{
				std::memmove(pNewBuffer, m_pBuffer, m_size * sizeof(TData));
			}
			else
			{
				for (size_t i = 0; i < m_size; ++i)
				{
					pNewBuffer[i] = Move(m_pBuffer[i]);
				}
			}

			JPT_DELETE_ARRAY(m_pBuffer);
		}

		m_pBuffer  = pNewBuffer;
		m_capacity = capacity;
	}

	template<typename _TData, class _TAllocator>
	template<Iterable TOtherContainer>
	constexpr void DynamicArray<_TData, _TAllocator>::CopyIterators(const TOtherContainer& container, size_t size)
	{
		m_size = size;
		UpdateBuffer(m_size);

		if constexpr (IsTriviallyCopyable<TData>)
		{
			std::memcpy(m_pBuffer, container.begin(), m_size * sizeof(TData));
		}
		else
		{
			size_t i = 0;
			for (auto itr = container.begin(); itr != container.end(); ++itr)
			{
				m_pBuffer[i++] = *itr;
			}
		}
	}
}