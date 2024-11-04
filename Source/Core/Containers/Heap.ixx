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
	template<Comparable _TData, typename _TComparator = Comparator_Less<_TData>, typename _TAllocator = Allocator<_TData>>
	class Heap
	{
	public:
		using TData       = _TData;
		using TComparator = _TComparator;

	public:
		static constexpr TComparator kComparator = TComparator();

	private:
		DynamicArray<TData, _TAllocator> m_buffer;	/** Dynamic Array storing the heap data */

	public:

	};
}