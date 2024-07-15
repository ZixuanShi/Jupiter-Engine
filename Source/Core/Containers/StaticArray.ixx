// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Assert.h"

#include <string>
#include <initializer_list>

export module jpt.StaticArray;

import jpt.Utilities;

export namespace jpt
{
	/** A sequence container that encapsulates fixed size arrays */
	template<typename _TData, size_t kCount>
	class StaticArray
	{
	public:
		using TData         = _TData;
		using Iterator      = TData*;
		using ConstIterator = const TData*;

	private:
		TData m_buffer[kCount];

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
		constexpr       TData* Buffer()            { return m_buffer; }
		constexpr       TData& Front()             { return m_buffer[0]; }
		constexpr const TData& Front()       const { return m_buffer[0]; }
		constexpr       TData& Back()              { return m_buffer[kCount - 1]; }
		constexpr const TData& Back()        const { return m_buffer[kCount - 1]; }
		constexpr       TData& operator[](size_t index)       { JPT_ASSERT(index < kCount); return m_buffer[index]; }
		constexpr const TData& operator[](size_t index) const { JPT_ASSERT(index < kCount); return m_buffer[index]; }

		// Iterators
		constexpr Iterator begin() { return Iterator(m_buffer); }
		constexpr Iterator end()   { return Iterator(m_buffer + kCount); }
		constexpr ConstIterator begin()  const { return ConstIterator(m_buffer); }
		constexpr ConstIterator cbegin() const { return ConstIterator(m_buffer); }
		constexpr ConstIterator end()    const { return ConstIterator(m_buffer + kCount); }
		constexpr ConstIterator cend()   const { return ConstIterator(m_buffer + kCount); }

		// Capacity
		constexpr size_t Count() const { return kCount; }
		constexpr size_t Size()  const { return kCount * sizeof(TData); }

	private:
		constexpr void CopyData(const TData* pBegin);
		constexpr void MoveData(StaticArray&& other);
	};

	template<typename TData, size_t kCount>
	constexpr bool operator==(const StaticArray<TData, kCount>& a, const StaticArray<TData, kCount>& b)
	{
		if (a.Count() != b.Count())
		{
			return false;
		}

		for (size_t i = 0; i < a.Count(); ++i)
		{
			if (a[i] != b[i])
			{
				return false;
			}
		}

		return true;
	}

	template<typename TData, size_t kCount>
	constexpr StaticArray<TData, kCount>::StaticArray(const std::initializer_list<TData>& initializerList)
	{
		JPT_ASSERT(initializerList.size() == kCount, "Initializer list size doesn't match");
		CopyData(initializerList.begin());
	}

	template<typename _TData, size_t kCount>
	constexpr StaticArray<_TData, kCount>::StaticArray(const StaticArray& other)
	{
		CopyData(other.ConstBuffer());
	}

	template<typename _TData, size_t kCount>
	constexpr StaticArray<_TData, kCount>::StaticArray(StaticArray&& other) noexcept
	{
		MoveData(Move(other));
	}

	template<typename _TData, size_t kCount>
	StaticArray<_TData, kCount>& StaticArray<_TData, kCount>::operator=(const StaticArray& other) noexcept
	{
		if (this != &other)
		{
			CopyData(other.ConstBuffer());
		}

		return *this;
	}

	template<typename _TData, size_t kCount>
	StaticArray<_TData, kCount>& StaticArray<_TData, kCount>::operator=(StaticArray&& other) noexcept
	{
		if (this != &other)
		{
			MoveData(Move(other));
		}

		return *this;
	}

	template<typename _TData, size_t kCount>
	constexpr StaticArray<_TData, kCount>::~StaticArray()
	{
		if constexpr (!std::is_trivially_destructible_v<TData>)
		{
			for (size_t i = 0; i < kCount; ++i)
			{
				m_buffer[i].~TData();
			}
		}
	}

	template<typename _TData, size_t kCount>
	constexpr void StaticArray<_TData, kCount>::CopyData(const TData* pBegin)
	{
		if constexpr (std::is_trivially_copyable_v<TData>)
		{
			std::memcpy(m_buffer, pBegin, kCount * sizeof(TData));
		}
		else
		{
			for (size_t i = 0; i < kCount; ++i, ++pBegin)
			{
				m_buffer[i] = *pBegin;
			}
		}
	}

	template<typename _TData, size_t kCount>
	constexpr void StaticArray<_TData, kCount>::MoveData(StaticArray&& other)
	{
		for (size_t i = 0; i < kCount; ++i)
		{
			m_buffer[i] = Move(other.m_buffer[i]);
		}
	}
}