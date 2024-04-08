// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.HeapSort;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.Utilities;

export namespace jpt
{
	template<Indexable TContainer, typename TComparator>
	constexpr void Heapify(TContainer& container, size_t beginIndex, size_t endIndex, size_t index, TComparator&& comparator)
	{
		size_t largest = index;
		const size_t left = 2 * (index - beginIndex) + 1 + beginIndex;
		const size_t right = 2 * (index - beginIndex) + 2 + beginIndex;

		if (left <= endIndex && comparator(container[largest], container[left]))
		{
			largest = left;
		}

		if (right <= endIndex && comparator(container[largest], container[right]))
		{
			largest = right;
		}

		if (largest != index)
		{
			Swap(container[index], container[largest]);
			Heapify(container, beginIndex, endIndex, largest, Move(comparator));
		}
	}

	template<Indexable TContainer, typename TComparator>
	constexpr void HeapSort(TContainer& container, size_t beginIndex, size_t endIndex, TComparator&& comparator)
	{
		for (int64 i = (beginIndex + endIndex) / 2; i >= static_cast<int64>(beginIndex); --i)
		{
			Heapify(container, beginIndex, endIndex, i, Move(comparator));
		}

		for (int64 i = endIndex; i > static_cast<int64>(beginIndex); --i)
		{
			Swap(container[beginIndex], container[i]);
			Heapify(container, beginIndex, i - 1, beginIndex, Move(comparator));
		}
	}
}