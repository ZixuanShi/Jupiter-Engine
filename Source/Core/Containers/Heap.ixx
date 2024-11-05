// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Heap;

import jpt.Allocator;
import jpt.Comparators;
import jpt.Concepts;
import jpt.DynamicArray;

export namespace jpt
{
	/** Heap data structure implementation
		@param _TComparator Comparator to use for the heap. Comparator_Less by means min-heap, Comparator_Greater by means max-heap	*/
	template<Comparable _TData, typename _TComparator = Comparator_Less<_TData>, typename _TAllocator = Allocator<_TData>>
	class Heap
	{
	public:
		using TData       = _TData;
		using TComparator = _TComparator;

	public:
		static constexpr TComparator kComparator = TComparator();

	private:
		/** Dynamic Array storing the heap data
			m_buffer[1] is the root of the heap
			Parent(i) = floor(i / 2)
			Left(i)   = 2 * i
			Right(i)  = 2 * i + 1		*/
		DynamicArray<TData, _TAllocator> m_buffer;	

	public:
		// Modifiers
		constexpr void Add(const TData& data);
	};

	template<Comparable TData, typename TComparator, typename TAllocator>
	constexpr void Heap<TData, TComparator, TAllocator>::Add(const TData& data)
	{
	}
}