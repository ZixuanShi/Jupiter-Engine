// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <initializer_list>

export module jpt.StaticArray;

import jpt.ToString;
import jpt_private.ContiguousIterator;

export namespace jpt
{
	template<typename _TData, size_t kSize>
	class StaticArray
	{
	public:
		using TData    = _TData;
		using Iterator = jpt_private::ContiguousIterator<TData>;

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
		constexpr       Iterator begin()        { return Iterator(m_buffer); }
		constexpr const Iterator begin()  const { return Iterator(m_buffer); }
		constexpr const Iterator cbegin() const { return Iterator(m_buffer); }

		constexpr       Iterator end()        { return Iterator(m_buffer + kSize); }
		constexpr const Iterator end()  const { return Iterator(m_buffer + kSize); }
		constexpr const Iterator cend() const { return Iterator(m_buffer + kSize); }

		// Capacity
		constexpr size_t Size() const { return kSize; }

		// Utils
		/** @return		A string contains all the data in this StaticArray */
		constexpr String ToString() const;

	private:
		constexpr void CopyData(const StaticArray& other);
		constexpr void MoveData(StaticArray&& other);
	};

	template<typename TData, size_t kSize>
	constexpr StaticArray<TData, kSize>::StaticArray(const std::initializer_list<TData>& initializerList)
	{
		JPT_ASSERT(initializerList.size() == kSize, "Initializer list size doesn't match");

		size_t i = 0;
		for (const auto& element : initializerList)
		{
			m_buffer[i++] = element;
		}
	}

	template<typename _TData, size_t kSize>
	constexpr StaticArray<_TData, kSize>::StaticArray(const StaticArray& other)
	{
		CopyData(other);
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
			CopyData(other);
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
		if constexpr (jpt::IsTriviallyDestructible<TData>)
		{
			for (size_t i = 0; i < kSize; ++i)
			{
				m_buffer[i].~TData();
			}
		}
	}

	template<typename _TData, size_t kSize>
	constexpr String StaticArray<_TData, kSize>::ToString() const
	{
		String str("{ ");

		for (size_t i = 0; i < kSize; ++i)
		{
			// Append ", " suffix if it's not the last element
			const char* pSuffix = (i == kSize - 1 ? "" : ", ");

			// Elements need to provide ToString() implementation to make this work
			str += jpt::ToString(m_buffer[i]) + pSuffix;
		}

		str.Append(" }");

		return str;
	}

	template<typename _TData, size_t kSize>
	constexpr void StaticArray<_TData, kSize>::CopyData(const StaticArray& other)
	{
		for (size_t i = 0; i < kSize; ++i)
		{
			m_buffer[i] = other.m_buffer[i];
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