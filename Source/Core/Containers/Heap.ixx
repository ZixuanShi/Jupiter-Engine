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

	private:
		/** Dynamic Array storing the heap elements
			Index 0 is not used for better calculation.
			m_buffer[1] is the root of the heap

			Parent(i) = i / 2
			Left(i)   = i * 2
			Right(i)  = i * 2 + 1	 */
		DynamicArray<TData, _TAllocator> m_buffer;	
		TComparator m_comparator = TComparator();

	public:
		constexpr Heap();
		constexpr Heap(TComparator&& comparator);

		// Adding
		template<typename ...TArgs>
		constexpr void Emplace(TArgs&&... args);
		constexpr void Add(const TData& data);
		constexpr void Add(TData&& data);

		// Accessing
		constexpr const TData& Top() const;
		constexpr size_t Count() const;
		constexpr bool IsEmpty() const;

		// Erasing
		constexpr void Pop();
		constexpr void Clear();

	private:
		// Called after modifying the heap to keep the heap property
		constexpr void FixAdd();
		constexpr void FixPop();

		// Helper functions for calculating the parent, left, and right index
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

	template<typename _TData, typename _TComparator, typename _TAllocator>
	constexpr Heap<_TData, _TComparator, _TAllocator>::Heap(TComparator&& comparator)
		: m_comparator(Move(comparator))
	{
		m_buffer.EmplaceBack();
	}

	template<typename TData, typename TComparator, typename TAllocator>
	template<typename ...TArgs>
	constexpr void Heap<TData, TComparator, TAllocator>::Emplace(TArgs&& ...args)
	{
		m_buffer.EmplaceBack(Forward<TArgs>(args)...);
		FixAdd();
	}

	template<typename TData, typename TComparator, typename TAllocator>
	constexpr void Heap<TData, TComparator, TAllocator>::Add(const TData& data)
	{
		m_buffer.Add(data);
		FixAdd();
	}

	template<typename TData, typename TComparator, typename TAllocator>
	constexpr void Heap<TData, TComparator, TAllocator>::Add(TData&& data)
	{
		m_buffer.Add(Move(data));
		FixAdd();
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
		
		// Swap the root with the last element then pop. Better performance
		Swap(m_buffer[1], m_buffer[Count()]);
		m_buffer.Pop();

		if (!IsEmpty())
		{
			FixPop();
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
	constexpr void Heap<_TData, _TComparator, _TAllocator>::FixAdd()
	{
		// Last added element will be at index Count()
		size_t currentIndex = Count();
		size_t parentIndex  = Parent(currentIndex);

		// Stop at the root
		while (currentIndex > 1)
		{
			// If the compare result is false, then the heap property is satisfied. Exit
			if (!m_comparator(m_buffer[currentIndex], m_buffer[parentIndex]))
			{
				return;
			}

			// Swap the current element with the parent regarding heap's property
			Swap(m_buffer[currentIndex], m_buffer[parentIndex]);

			// Update the indices, keep swapping
			currentIndex = parentIndex;
			parentIndex  = Parent(currentIndex);
		}
	}

	template<typename _TData, typename _TComparator, typename _TAllocator>
	constexpr void Heap<_TData, _TComparator, _TAllocator>::FixPop()
	{
		// Last element is now at the root
		size_t currentIndex = 1;
		size_t leftIndex    = Left(currentIndex);
		size_t rightIndex   = Right(currentIndex);

		// Stops when there is no child to compare
		while (leftIndex <= Count())
		{
			// Find a better target child to swap
			size_t targetIndex = leftIndex;
			if (rightIndex <= Count() && m_comparator(m_buffer[rightIndex], m_buffer[leftIndex]))
			{
				targetIndex = rightIndex;
			}

			// If the compare result is false, then the heap property is satisfied. Exit
			if (!m_comparator(m_buffer[targetIndex], m_buffer[currentIndex]))
			{
				return;
			}

			// Heap property is not satisfied, swap the current element with the target
			Swap(m_buffer[targetIndex], m_buffer[currentIndex]);

			currentIndex = targetIndex;
			leftIndex    = Left(currentIndex);
			rightIndex   = Right(currentIndex);
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

	template<typename TData, typename TComparator = Comparator_Less<TData>, typename TAllocator = Allocator<TData>>
	using PriorityQueue = Heap<TData, TComparator, TAllocator>;
}