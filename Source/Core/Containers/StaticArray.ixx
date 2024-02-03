// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <string>
#include <initializer_list>

export module jpt.StaticArray;

import jpt.Utilities;
import jpt_private.ContiguousIterator;

export namespace jpt
{
	/** A sequence container that encapsulates fixed size arrays */
	template<typename _TData, size_t kSize>
	class StaticArray
	{
	public:
		using TData         = _TData;
		using Iterator      = jpt_private::ContiguousIterator<TData>;
		using ConstIterator = jpt_private::ConstContiguousIterator<TData>;

	private:
		TData m_buffer[kSize];

	public:
		constexpr StaticArray() = default;
		constexpr StaticArray(const std::initializer_list<TData>& initializerList);
		constexpr StaticArray(const StaticArray& other);
		constexpr StaticArray(StaticArray&& other) noexcept;
		StaticArray& operator=(const StaticArray& other) noexcept;
		StaticArray& operator=(StaticArray&& other) noexcept;
		constexpr ~StaticArray();

		// Element Access
		constexpr const TData* ConstBuffer() const { return m_buffer; }
		constexpr       TData* Buffer()      const { return m_buffer; }
		constexpr       TData& Front()             { return m_buffer[0]; }
		constexpr const TData& Front()       const { return m_buffer[0]; }
		constexpr       TData& Back()              { return m_buffer[kSize - 1]; }
		constexpr const TData& Back()        const { return m_buffer[kSize - 1]; }
		constexpr       TData& operator[](size_t index)       { return m_buffer[index]; }
		constexpr const TData& operator[](size_t index) const { return m_buffer[index]; }

		// Iterators
		constexpr Iterator begin() { return Iterator(m_buffer); }
		constexpr Iterator end()   { return Iterator(m_buffer + kSize); }
		constexpr ConstIterator begin()  const { return ConstIterator(m_buffer); }
		constexpr ConstIterator cbegin() const { return ConstIterator(m_buffer); }
		constexpr ConstIterator end()    const { return ConstIterator(m_buffer + kSize); }
		constexpr ConstIterator cend()   const { return ConstIterator(m_buffer + kSize); }

		// Capacity
		constexpr size_t Size() const { return kSize; }

	private:
		constexpr void CopyData(const TData* pBegin);
		constexpr void MoveData(StaticArray&& other);
	};

	template<typename TData, size_t kSize>
	constexpr StaticArray<TData, kSize>::StaticArray(const std::initializer_list<TData>& initializerList)
	{
		JPT_ASSERT(initializerList.size() == kSize, "Initializer list size doesn't match");
		CopyData(initializerList.begin());
	}

	template<typename _TData, size_t kSize>
	constexpr StaticArray<_TData, kSize>::StaticArray(const StaticArray& other)
	{
		CopyData(other.ConstBuffer());
	}

	template<typename _TData, size_t kSize>
	constexpr StaticArray<_TData, kSize>::StaticArray(StaticArray&& other) noexcept
	{
		MoveData(Move(other));
	}

	template<typename _TData, size_t kSize>
	StaticArray<_TData, kSize>& StaticArray<_TData, kSize>::operator=(const StaticArray& other) noexcept
	{
		if (this != &other)
		{
			CopyData(other.ConstBuffer());
		}

		return *this;
	}

	template<typename _TData, size_t kSize>
	StaticArray<_TData, kSize>& StaticArray<_TData, kSize>::operator=(StaticArray&& other) noexcept
	{
		if (this != &other)
		{
			MoveData(Move(other));
		}

		return *this;
	}

	template<typename _TData, size_t kSize>
	constexpr StaticArray<_TData, kSize>::~StaticArray()
	{
		if constexpr (!IsTriviallyDestructible<TData>)
		{
			for (size_t i = 0; i < kSize; ++i)
			{
				m_buffer[i].~TData();
			}
		}
	}

	template<typename _TData, size_t kSize>
	constexpr void StaticArray<_TData, kSize>::CopyData(const TData* pBegin)
	{
		if constexpr (IsTriviallyCopyable<TData>)
		{
			std::memcpy(m_buffer, pBegin, kSize * sizeof(TData));
		}
		else
		{
			for (size_t i = 0; i < kSize; ++i, ++pBegin)
			{
				m_buffer[i] = *pBegin;
			}
		}
	}

	template<typename _TData, size_t kSize>
	constexpr void StaticArray<_TData, kSize>::MoveData(StaticArray&& other)
	{
		for (size_t i = 0; i < kSize; ++i)
		{
			m_buffer[i] = Move(other.m_buffer[i]);
		}
	}
}