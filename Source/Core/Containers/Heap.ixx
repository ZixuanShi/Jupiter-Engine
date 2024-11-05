// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Heap;

import jpt.Allocator;
import jpt.Comparators;
import jpt.Concepts;
import jpt.Constants;
import jpt.DynamicArray;
import jpt.Utilities;
import jpt.Math;

export namespace jpt
{
	/** Heap data structure implementation
		@param _TComparator Comparator to use for the heap. Comparator_Less by means min-heap, Comparator_Greater by means max-heap	*/
	template<typename _TData, typename _TComparator = Comparator_Less<_TData>, typename _TAllocator = Allocator<_TData>>
	class Heap
	{
	public:
		using TData       = _TData;
		using TComparator = _TComparator;

	public:
		static constexpr TComparator kComparator = TComparator();

	//private:
		/** Dynamic Array storing the heap data
			m_buffer[1] is the root of the heap
			Parent(i) = floor(i / 2)
			Left(i)   = 2 * i
			Right(i)  = 2 * i + 1		*/
		DynamicArray<TData, _TAllocator> m_buffer;	

	public:
		constexpr Heap();

		template<typename ...TArgs>
		constexpr void Emplace(TArgs&&... args);
		constexpr void Add(const TData& data);
		constexpr void Add(TData&& data);

		constexpr const TData& Top() const;

		constexpr void Pop();
		constexpr void Clear();

		constexpr size_t Count() const;
		constexpr bool IsEmpty() const;

	private:
		/** Fix the heap after adding an element
			@param index	Index of the element to fix	*/
		constexpr void FixAdd(size_t index);

		constexpr void FixPop(size_t index);

		constexpr size_t Parent(size_t index) const;
		constexpr size_t Left(size_t index) const;
		constexpr size_t Right(size_t index) const;
	};

	template<typename TData, typename TComparator, typename TAllocator>
	constexpr Heap<TData, TComparator, TAllocator>::Heap()
	{
		// Reserve the first element as a dummy element
		m_buffer.EmplaceBack();
	}

	template<typename TData, typename TComparator, typename TAllocator>
	template<typename ...TArgs>
	constexpr void Heap<TData, TComparator, TAllocator>::Emplace(TArgs&& ...args)
	{
		m_buffer.EmplaceBack(Forward<TArgs>(args)...);
		FixAdd(Count());
	}

	template<typename TData, typename TComparator, typename TAllocator>
	constexpr void Heap<TData, TComparator, TAllocator>::Add(const TData& data)
	{
		m_buffer.Add(data);
		FixAdd(Count());
	}

	template<typename TData, typename TComparator, typename TAllocator>
	constexpr void Heap<TData, TComparator, TAllocator>::Add(TData&& data)
	{
		m_buffer.Add(Move(data));
		FixAdd(Count());
	}

	template<typename TData, typename TComparator, typename TAllocator>
	constexpr const TData& Heap<TData, TComparator, TAllocator>::Top() const
	{
		JPT_ASSERT(!IsEmpty());
		return m_buffer[1];
	}

	template<typename TData, typename TComparator, typename TAllocator>
	constexpr void Heap<TData, TComparator, TAllocator>::Pop()
	{
		JPT_ASSERT(!IsEmpty());
		
		// Swap the root with the last element
		Swap(m_buffer[1], m_buffer[Count()]);
		m_buffer.Pop();

		if (!IsEmpty())
		{
			FixPop(1);
		}
	}

	template<typename TData, typename TComparator, typename TAllocator>
	constexpr void Heap<TData, TComparator, TAllocator>::Clear()
	{
		m_buffer.Clear();
		m_buffer.EmplaceBack();
	}

	template<typename TData, typename TComparator, typename TAllocator>
	constexpr size_t Heap<TData, TComparator, TAllocator>::Count() const
	{
		return m_buffer.Count() - 1;
	}

	template<typename TData, typename TComparator, typename TAllocator>
	constexpr bool Heap<TData, TComparator, TAllocator>::IsEmpty() const
	{
		return Count() == 0;
	}

	template<typename _TData, typename _TComparator, typename _TAllocator>
	constexpr void Heap<_TData, _TComparator, _TAllocator>::FixAdd(size_t index)
	{
		size_t parentIndex = Parent(index);

		while (index > 1 && kComparator(m_buffer[index], m_buffer[parentIndex]))
		{
			Swap(m_buffer[index], m_buffer[parentIndex]);
			index = parentIndex;
			parentIndex = Parent(index);
		}
	}

	template<typename _TData, typename _TComparator, typename _TAllocator>
	constexpr void Heap<_TData, _TComparator, _TAllocator>::FixPop(size_t index)
	{
		size_t leftIndex = Left(index);
		size_t rightIndex = Right(index);

		while (leftIndex <= Count())
		{
			size_t smallerIndex = leftIndex;
			if (rightIndex <= Count() && kComparator(m_buffer[rightIndex], m_buffer[leftIndex]))
			{
				smallerIndex = rightIndex;
			}

			if (kComparator(m_buffer[smallerIndex], m_buffer[index]))
			{
				Swap(m_buffer[smallerIndex], m_buffer[index]);
				index = smallerIndex;
				leftIndex = Left(index);
				rightIndex = Right(index);
			}
			else
			{
				break;
			}
		}
	}

	template<typename _TData, typename _TComparator, typename _TAllocator>
	constexpr size_t Heap<_TData, _TComparator, _TAllocator>::Parent(size_t index) const
	{
		// if index is 1, then it is the root
		if (index == 1)
			return 1;

		return index / 2;
	}

	template<typename _TData, typename _TComparator, typename _TAllocator>
	constexpr size_t Heap<_TData, _TComparator, _TAllocator>::Left(size_t index) const
	{
		return 2 * index;
	}

	template<typename _TData, typename _TComparator, typename _TAllocator>
	constexpr size_t Heap<_TData, _TComparator, _TAllocator>::Right(size_t index) const
	{
		return 2 * index + 1;
	}

	template<typename TData, typename TAllocator = Allocator<TData>>
	using MinHeap = Heap<TData, Comparator_Less<TData>, TAllocator>;

	template<typename TData, typename TAllocator = Allocator<TData>>
	using MaxHeap = Heap<TData, Comparator_Greater<TData>, TAllocator>;
}