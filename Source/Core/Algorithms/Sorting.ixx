// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

export module jpt.Sorting;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.Function;
import jpt.Math;
import jpt.Utilities;
import jpt.Limits;

namespace jpt_private
{
	using namespace jpt;

#pragma region QuickSort
	template<typename TContainer>
	constexpr size_t GetPivot(TContainer& container, size_t beginIndex, size_t endIndex)
	{
		// Mid of three pivot selection

		// Get each elements
		typename TContainer::TData first = container[beginIndex];
		typename TContainer::TData mid = container[endIndex / 2];
		typename TContainer::TData last = container[beginIndex];

		// Compare and return mid index
		if (first > mid)
		{
			if (mid > last)
				return endIndex / 2;
			else if (first > last)
				return endIndex;
			else
				return beginIndex;
		}
		else
		{
			if (first > last)
				return beginIndex;
			else if (mid > last)
				return endIndex;
			else
				return endIndex / 2;
		}
	}

	template<typename TContainer, typename Comparator>
	constexpr size_t Partition(TContainer& container, size_t beginIndex, size_t endIndex, Comparator&& comparator)
	{
		size_t pivotIndex = GetPivot(container, beginIndex, endIndex);
		Swap(container[pivotIndex], container[endIndex]);

		typename TContainer::TData pivot = container[endIndex];
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

	template<typename TContainer, typename Comparator>
	constexpr void QuickSort(TContainer& container, size_t beginIndex, size_t endIndex, Comparator&& comparator)
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
}

namespace jpt
{
	template<Trivial T>
	constexpr bool DefaultTrivialComparator(T a, T b) { return a < b; }

	template<NonTrivial T>
	constexpr bool DefaultNonTrivialComparator(const T& a, const T& b) { return a < b; }

	export template<ContainingTrivial TContainer>
	constexpr void Sort(TContainer& container, Function<bool(typename TContainer::TData, typename TContainer::TData)>&& comparator = DefaultTrivialComparator<typename TContainer::TData>)
	{
		jpt_private::QuickSort(container, 0, container.Size() - 1, Move(comparator));
	}

	export template<ContainingNonTrivial TContainer>
	constexpr void Sort(TContainer& container, Function<bool(const typename TContainer::TData&, const typename TContainer::TData&)>&& comparator = DefaultNonTrivialComparator<typename TContainer::TData>)
	{
		jpt_private::QuickSort(container, 0, container.Size() - 1, Move(comparator));
	}
}