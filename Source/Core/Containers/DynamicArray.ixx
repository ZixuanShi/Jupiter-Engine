// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

#include <initializer_list>
#include <string>

export module jpt.DynamicArray;

import jpt.Allocator;
import jpt.Concepts;
import jpt.TypeTraits;
import jpt.Utilities;
import jpt.Math;

static constexpr size_t kLocGrowMultiplier = 2;

export namespace jpt
{
	/** A sequence container that encapsulates dynamic size arrays. */
	template<typename _TData, typename TAllocator = Allocator<_TData>>
	class DynamicArray
	{
	public:
		using TData         = _TData;
		using Iterator      = TData*;
		using ConstIterator = const TData*;

	private:
		TData* m_pBuffer  = nullptr;
		size_t m_size     = 0;
		size_t m_capacity = 0;

	public:
		constexpr DynamicArray() = default;
		constexpr DynamicArray(const std::initializer_list<TData>& list);
		constexpr DynamicArray(const DynamicArray& other);
		constexpr DynamicArray(DynamicArray&& other) noexcept;
		DynamicArray& operator=(const DynamicArray& other) noexcept;
		DynamicArray& operator=(DynamicArray&& other) noexcept;
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
		constexpr       TData& At(size_t index)       { JPT_ASSERT(index < m_size); return m_pBuffer[index]; }
		constexpr const TData& At(size_t index) const { JPT_ASSERT(index < m_size); return m_pBuffer[index]; }

		// Iterators
		constexpr Iterator begin() noexcept { return Iterator(m_pBuffer); }
		constexpr Iterator end()   noexcept { return Iterator(m_pBuffer + m_size); }
		constexpr ConstIterator begin()  const noexcept { return ConstIterator(m_pBuffer); }
		constexpr ConstIterator end()    const noexcept { return ConstIterator(m_pBuffer + m_size); }
		constexpr ConstIterator cbegin() const noexcept { return ConstIterator(m_pBuffer); }
		constexpr ConstIterator cend()   const noexcept { return ConstIterator(m_pBuffer + m_size); }

		// Capacity
		constexpr size_t Size()     const { return m_size;      }
		constexpr size_t Capacity() const { return m_capacity;  }
		constexpr bool   IsEmpty()  const { return m_size == 0; }
		constexpr void Reserve(size_t capacity);
		constexpr void ShrinkToFit();

		// Modifiers
		constexpr void Clear();
		constexpr void Resize(size_t size, const TData& data = TData());

		// Inserting
		constexpr void Insert(size_t index, const TData& data);
		constexpr void Insert(size_t index, TData&& data);
		constexpr void PushBack(const TData& data);
		constexpr void PushBack(TData&& data);
		template<typename ...TArgs>
		constexpr TData& Emplace(size_t index, TArgs&&... args);
		template<typename ...TArgs>
		constexpr TData& EmplaceBack(TArgs&&... args);

		// Erasing
		constexpr void Erase(size_t index);
		constexpr void PopBack();

	private:
		/** Create a new data buffer with a new capacity, move the existing data over */
		constexpr void UpdateBuffer(size_t capacity);
		
		/** Copy other's data through it's iterators. TOtherContainer should be guaranteed to provide iterator interfaces */
		constexpr void CopyData(const TData* pBegin, size_t size);

		/** Moves the data from a deprecating dynamic array */
		constexpr void MoveData(DynamicArray&& other);

		/** Moves data towards a direction starting at given index for the distance provided */
		constexpr void ShiftDataToEnd(size_t index, size_t distance = 1);
		constexpr void ShiftDataToBegin(size_t index, size_t distance = 1);

		/** Called before inserting operation */
		constexpr void UpdateBufferForInsert(size_t index);
	};

	template<typename TData, typename TAllocator>
	constexpr bool operator==(const DynamicArray<TData, TAllocator>& a, const DynamicArray<TData, TAllocator>& b)
	{
		if (a.Size() != b.Size())
		{
			return false;
		}

		for (size_t i = 0; i < a.Size(); ++i)
		{
			if (a[i] != b[i])
			{
				return false;
			}
		}

		return true;
	}

	template<typename TData, typename TAllocator>
	constexpr DynamicArray<TData, TAllocator>::DynamicArray(const std::initializer_list<TData>& list)
	{
		CopyData(list.begin(), list.size());
	}

	template<typename TData, typename TAllocator>
	constexpr DynamicArray<TData, TAllocator>::DynamicArray(const DynamicArray& other)
	{
		CopyData(other.ConstBuffer(), other.Size());
	}

	template<typename TData, typename TAllocator>
	constexpr DynamicArray<TData, TAllocator>::DynamicArray(DynamicArray&& other) noexcept
	{
		MoveData(Move(other));
	}

	template<typename TData, typename TAllocator>
	DynamicArray<TData, TAllocator>& DynamicArray<TData, TAllocator>::operator=(const DynamicArray& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			CopyData(other.ConstBuffer(), other.Size());
		}

		return *this;
	}

	template<typename TData, typename TAllocator>
	DynamicArray<TData, TAllocator>& DynamicArray<TData, TAllocator>::operator=(DynamicArray&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			MoveData(Move(other));
		}

		return *this;
	}

	template<typename TData, typename TAllocator>
	constexpr DynamicArray<TData, TAllocator>::~DynamicArray()
	{
		Clear();
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::Reserve(size_t capacity)
	{
		if (m_capacity < capacity)
		{
			UpdateBuffer(capacity);
		}
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::Clear()
	{
		if constexpr (!IsTriviallyDestructible<TData>)
		{
			for (size_t i = 0; i < m_size; ++i)
			{
				TAllocator::Destruct(m_pBuffer + i);
			}
		}

		JPT_SAFE_DELETE_ARRAY(m_pBuffer);
		m_size = 0;
		m_capacity = 0;
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::Resize(size_t size, const TData& data /*= TData()*/)
	{
		// If size if less than m_size, shrink
		if (size < m_size)
		{
			for (size_t i = size; i < m_size; ++i)
			{
				PopBack();
			}
		}
		// If size if greater than m_size, grow
		else if (size > m_size)
		{
			Reserve(size);
			for (size_t i = m_size; i < size; ++i)
			{
				EmplaceBack(data);
			}
		}

		m_size = size;
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::Insert(size_t index, const TData& data)
	{
		UpdateBufferForInsert(index);
		TAllocator::Construct(m_pBuffer + index, data);
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::Insert(size_t index, TData&& data)
	{
		UpdateBufferForInsert(index);
		TAllocator::Construct(m_pBuffer + index, Move(data));
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::PushBack(const TData& data)
	{
		Insert(m_size, data);
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::PushBack(TData&& data)
	{
		Insert(m_size, Move(data));
	}

	template<typename TData, typename TAllocator>
	template<typename ...TArgs>
	constexpr DynamicArray<TData, TAllocator>::TData& DynamicArray<TData, TAllocator>::Emplace(size_t index, TArgs&& ...args)
	{
		UpdateBufferForInsert(index);
		TAllocator::Construct(m_pBuffer + index, Forward<TArgs>(args)...);
		return m_pBuffer[index];
	}

	template<typename TData, typename TAllocator>
	template<typename ...TArgs>
	constexpr DynamicArray<TData, TAllocator>::TData& DynamicArray<TData, TAllocator>::EmplaceBack(TArgs&& ...args)
	{
		return Emplace(m_size, Forward<TArgs>(args)...);
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::Erase(size_t index)
	{
		JPT_ASSERT(index <= m_size, "Calling Erase() with an invalid index");

		if constexpr (!IsTriviallyDestructible<TData>)
		{
			TAllocator::Destruct(m_pBuffer + index);
		}

		--m_size;
		ShiftDataToBegin(index);
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::PopBack()
	{
		Erase(m_size - 1);
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::ShrinkToFit()
	{
		UpdateBuffer(m_size);
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::UpdateBuffer(size_t capacity)
	{
		static constexpr size_t kMinCapacity = 4;
		capacity = Max(capacity, kMinCapacity);

		TData* pNewBuffer = new TData[capacity];

		if (m_pBuffer)
		{
			if constexpr (IsTriviallyMoveAssignable<TData> && IsTriviallyMoveConstructible<TData>)
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

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::MoveData(DynamicArray&& other)
	{
		m_pBuffer  = other.m_pBuffer;
		m_capacity = other.m_capacity;
		m_size     = other.m_size;

		other.m_pBuffer  = nullptr;
		other.m_capacity = 0;
		other.m_size     = 0;
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::ShiftDataToEnd(size_t index, size_t distance /*= 1*/)
	{
		JPT_ASSERT(index <= m_size, "Distance went beyond the bound of this vector. Use reserve first");

		if constexpr (IsTriviallyMoveAssignable<TData> && IsTriviallyMoveConstructible<TData>)
		{
			std::memmove(m_pBuffer + index + distance, m_pBuffer + index, (m_size - index) * sizeof(TData));
		}
		else
		{
			for (size_t i = m_size; i > index; --i)
			{
				m_pBuffer[i] = Move(m_pBuffer[i - distance]);
			}
		}
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::ShiftDataToBegin(size_t index, size_t distance)
	{
		JPT_ASSERT(index - distance >= 0, "Distance went beyond the start of this vector. Use smaller index or distance");

		if constexpr (IsTriviallyMoveAssignable<TData> && IsTriviallyMoveConstructible<TData>)
		{
			std::memmove(m_pBuffer + index, m_pBuffer + index + distance, (m_size - index) * sizeof(TData));
		}
		else
		{
			for (size_t i = index; i < m_size; ++i)
			{
				m_pBuffer[i] = Move(m_pBuffer[i + distance]);
			}
		}
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::UpdateBufferForInsert(size_t index)
	{
		JPT_ASSERT(index <= m_size, "Calling DynamicArray::Insert() with an invalid index");

		if (m_size >= m_capacity)
		{
			UpdateBuffer(m_size * kLocGrowMultiplier);
		}

		ShiftDataToEnd(index);
		++m_size;
	}

	template<typename TData, typename TAllocator>
	constexpr void DynamicArray<TData, TAllocator>::CopyData(const TData* pBegin, size_t size)
	{
		m_size = size;
		UpdateBuffer(m_size);

		if constexpr (IsTriviallyCopyable<TData>)
		{
			std::memcpy(m_pBuffer, pBegin, m_size * sizeof(TData));
		}
		else
		{
			for (size_t i = 0; i < size; ++i, ++pBegin)
			{
				m_pBuffer[i] = *pBegin;
			}
		}
	}
}