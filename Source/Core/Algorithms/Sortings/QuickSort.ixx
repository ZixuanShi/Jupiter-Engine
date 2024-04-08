// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.QuickSort;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.Limits;
import jpt.Utilities;

export namespace jpt
{
	template<Indexable TContainer>
	constexpr size_t GetPivot(TContainer& container, size_t beginIndex, size_t endIndex)
	{
		// Mid of three pivot selection

		// Get each elements
		const auto first = container[beginIndex];
		const auto mid   = container[endIndex / 2];
		const auto last  = container[beginIndex];

		// Compare and return mid index
		if (first > mid)
		{
			if (mid > last)
			{
				return endIndex / 2;
			}
			else if (first > last)
			{
				return endIndex;
			}
			else
			{
				return beginIndex;
			}
		}
		else
		{
			if (first > last)
			{
				return beginIndex;
			}
			else if (mid > last)
			{
				return endIndex;
			}
			else
			{
				return endIndex / 2;
			}
		}
	}

	template<Indexable TContainer, typename TComparator>
	constexpr size_t Partition(TContainer& container, size_t beginIndex, size_t endIndex, TComparator&& comparator)
	{
		size_t pivotIndex = GetPivot(container, beginIndex, endIndex);
		Swap(container[pivotIndex], container[endIndex]);

		const auto pivot = container[endIndex];
		size_t i = beginIndex - 1;	// i is the last element's index of region 1, which is less than the pivot

		// j is the current processing element's index
		for (size_t j = beginIndex; j < endIndex; ++j)
		{
			// if current element should be placed to region one
			// Swap last element in region 1 with current processing element.
			if (comparator(container[j], pivot))
			{
				++i;
				Swap(container[i], container[j]);
			}
		}

		// Everything is in its place except for the pivot. We swap the pivot with the first element of region 2.
		pivotIndex = i + 1;
		Swap(container[pivotIndex], container[endIndex]);

		// return the pivot, which becomes the beginning and end points of the next calls to Partition().
		return pivotIndex;
	}

	template<Indexable TContainer, typename TComparator>
	constexpr void QuickSort(TContainer& container, size_t beginIndex, size_t endIndex, TComparator&& comparator)
	{
		// Bounds check
		if (beginIndex >= endIndex || endIndex == Limits<size_t>::kMax)
		{
			return;
		}

		// Recursively sort the left and right partitions
		const size_t pivot = Partition(container, beginIndex, endIndex, Move(comparator));
		QuickSort(container, beginIndex, pivot - 1, Move(comparator));
		QuickSort(container, pivot + 1, endIndex, Move(comparator));
	}
}