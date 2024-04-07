// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <cmath>

export module jpt.Sorting;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.Function;
import jpt.Math;
import jpt.Utilities;
import jpt.Limits;

export namespace jpt
{
	template<typename T = void>
	struct Comparator_Less
	{
		template<NonTrivial U = T>
		constexpr bool operator()(const U& lhs, const U& rhs)
		{
			return lhs < rhs;
		}

		template<Trivial U = T>
		constexpr bool operator()(U lhs, U rhs)
		{
			return lhs < rhs;
		}
	};

	template<typename T = void>
	struct Comparator_Greater
	{
		template<NonTrivial U = T>
		constexpr bool operator()(const U& lhs, const U& rhs)
		{
			return lhs > rhs;
		}

		template<Trivial U = T>
		constexpr bool operator()(U lhs, U rhs)
		{
			return lhs > rhs;
		}
	};

#pragma region InsertionSort
	template<Indexable TContainer, typename TComparator>
	constexpr void InsertionSort(TContainer& container, size_t beginIndex, size_t endIndex, TComparator&& comparator)
	{
		for (size_t i = beginIndex + 1; i <= endIndex; ++i)
		{
			const auto key = container[i];
			int64 j = static_cast<int64>(i);

			while (j > static_cast<int64>(beginIndex) && comparator(key, container[j - 1]))
			{
				container[j] = container[j - 1];
				--j;
			}

			container[j] = key;
		}
	}
#pragma endregion

#pragma region QuickSort
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
		if (beginIndex >= endIndex || endIndex == LimitsOf<size_t>::kMax)
		{
			return;
		}

		// Recursively sort the left and right partitions
		const size_t pivot = Partition(container, beginIndex, endIndex, Move(comparator));
		QuickSort(container, beginIndex, pivot - 1, Move(comparator));
		QuickSort(container, pivot + 1, endIndex, Move(comparator));
	}
#pragma endregion

#pragma region HeapSort

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

#pragma endregion

#pragma region IntroSort

	template<Indexable TContainer, typename TComparator>
	constexpr void IntroSort(TContainer& container, size_t beginIndex, size_t endIndex, size_t depth, TComparator&& comparator)
	{
		static constexpr size_t kInsertionSortThreshold = 16;

		if (beginIndex >= endIndex || endIndex == LimitsOf<size_t>::kMax)
		{
			return;
		}

		// If the size of the partition is less than or equal to kInsertionSortThreshold, use insertion sort
		if (endIndex - beginIndex <= kInsertionSortThreshold)
		{
			InsertionSort(container, beginIndex, endIndex, Move(comparator));
		}
		// If the depth is 0, use heap sort
		else if (depth == 0)
		{
			HeapSort(container, beginIndex, endIndex, Move(comparator));
		}
		// Otherwise, use quick sort
		else
		{
			const size_t pivot = Partition(container, beginIndex, endIndex, Move(comparator));
			IntroSort(container, beginIndex, pivot - 1, depth - 1, Move(comparator));
			IntroSort(container, pivot + 1, endIndex, depth - 1, Move(comparator));
		}
	}

	template<Indexable TContainer, typename TComparator>
	constexpr void IntroSort(TContainer& container, size_t beginIndex, size_t endIndex, TComparator&& comparator)
	{
		const size_t depth = static_cast<size_t>(std::log2(endIndex - beginIndex + 1) * 2);
		IntroSort(container, beginIndex, endIndex, depth, Move(comparator));
	}

#pragma endregion

	// Jupiter's containers with Size() function
	template<typename TContainer, typename TComparator = Comparator_Less<void>> requires Sized<TContainer> && Indexable<TContainer>
	constexpr void Sort(TContainer& container, TComparator&& comparator = TComparator())
	{
		IntroSort(container, 0, container.Size() - 1, Move(comparator));
	}

	// C++ style plain array
	template<Indexable TContainer, typename TComparator = Comparator_Less<void>>
	constexpr void Sort(TContainer& container, TComparator&& comparator = TComparator())
	{
		IntroSort(container, 0, JPT_ARRAY_COUNT(container) - 1, Move(comparator));
	}

	// Container/Plain Array with start/end index
	template<Indexable TContainer, typename TComparator = Comparator_Less<void>>
	constexpr void Sort(TContainer& container, size_t beginIndex, size_t endIndex, TComparator&& comparator = TComparator())
	{
		IntroSort(container, beginIndex, endIndex, Move(comparator));
	}

	// Container/Plain Array from 0 to end index
	template<Indexable TContainer, typename TComparator = Comparator_Less<void>>
	constexpr void Sort(TContainer& container, size_t size, TComparator&& comparator = TComparator())
	{
		IntroSort(container, 0, size - 1, Move(comparator));
	}
}