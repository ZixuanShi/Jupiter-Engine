// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <initializer_list>
#include <string>

export module jpt.DynamicArray;

import jpt.Allocator;
import jpt.Concepts;
import jpt.TypeTraits;
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
		using Iterator   = jpt_private::ContiguousIterator<TData>;

	private:
		TData* m_pBuffer  = nullptr;
		size_t m_size     = 0;
		size_t m_capacity = 0;

	public:
		constexpr DynamicArray() = default;
		constexpr DynamicArray(const std::initializer_list<TData>& list);
		constexpr DynamicArray(const DynamicArray& other);
		constexpr ~DynamicArray();

		// Element Access
		constexpr const TData* ConstBuffer() const { return m_pBuffer; }
		constexpr       TData* Buffer()      const { return m_pBuffer; }
		constexpr       TData& Front()             { return m_pBuffer[0]; }
		constexpr const TData& Front()       const { return m_pBuffer[0]; }
		constexpr       TData& Back()              { return m_pBuffer[m_size - 1]; }
		constexpr const TData& Back()        const { return m_pBuffer[m_size - 1]; }
		constexpr       TData& operator[](size_t index)       { return m_pBuffer[index]; }
		constexpr const TData& operator[](size_t index) const { return m_pBuffer[index]; }

		// Iterators
		constexpr       Iterator begin()        { return Iterator(m_pBuffer); }
		constexpr const Iterator begin()  const { return Iterator(m_pBuffer); }
		constexpr const Iterator cbegin() const { return Iterator(m_pBuffer); }
		constexpr       Iterator end()        { return Iterator(m_pBuffer + m_size); }
		constexpr const Iterator end()  const { return Iterator(m_pBuffer + m_size); }
		constexpr const Iterator cend() const { return Iterator(m_pBuffer + m_size); }

		// Capacity
		constexpr size_t Size()     const { return m_size;      }
		constexpr size_t Capacity() const { return m_capacity;  }
		constexpr bool   IsEmpty()  const { return m_size == 0; }

		// Utils
		/** @return		A string contains all the data in this StaticArray */
		constexpr String ToString() const;

	private:
		/** Create a new data buffer with a new capacity, move the existing data over */
		constexpr void UpdateBuffer(size_t capacity);
		
		/** Copy other's data through it's iterators. TOtherContainer should be guaranteed to provide iterator interfaces */
		template<Iterable TContainer>
		constexpr void CopyData(const TContainer& container, size_t size);
	};

	template<typename TData, class TAllocator>
	constexpr DynamicArray<TData, TAllocator>::DynamicArray(const std::initializer_list<TData>& list)
	{
		CopyData(list, list.size());
	}

	template<typename _TData, class _TAllocator>
	constexpr DynamicArray<_TData, _TAllocator>::DynamicArray(const DynamicArray& other)
	{
		CopyData(other, other.Size());
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
	constexpr String DynamicArray<_TData, _TAllocator>::ToString() const
	{
		String str("{ ");

		for (size_t i = 0; i < m_size; ++i)
		{
			// Append ", " suffix if it's not the last element
			const char* pSuffix = (i == m_size - 1 ? "" : ", ");

			// Elements need to provide ToString() implementation to make this work
			str += jpt::ToString(m_pBuffer[i]) + pSuffix;
		}

		str.Append(" }");

		return str;
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
	template<Iterable TContainer>
	constexpr void DynamicArray<_TData, _TAllocator>::CopyData(const TContainer& container, size_t size)
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