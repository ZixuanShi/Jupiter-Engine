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

namespace jpt
{
	template<typename TContainer>
	size_t GetPivot(TContainer& container, size_t beginIndex, size_t endIndex)
	{
		// Mid of three pivot selection
		return endIndex;
	}

	template<typename TContainer, typename Comparator>
	size_t Partition(TContainer& container, size_t beginIndex, size_t endIndex, Comparator&& comparator)
	{
		typename TContainer::TData pivot = container[endIndex];
		size_t i = beginIndex - 1;	// i is the last element's index of region 1, which is less than the pivot

		for (size_t j = beginIndex; j < endIndex; ++j)
		{
			if (comparator(container[j], pivot))
			{
				++i;
				Swap(container[i], container[j]);
			}
		}

		size_t pivotIndex = i + 1;
		Swap(container[pivotIndex], container[endIndex]);

		return pivotIndex;
	}

	template<typename TContainer, typename Comparator>
	void QuickSort(TContainer& container, size_t beginIndex, size_t endIndex, Comparator&& comparator)
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

	template<Trivial T>
	constexpr bool DefaultTrivialComparator(T a, T b) { return a < b; }

	template<NonTrivial T>
	constexpr bool DefaultNonTrivialComparator(const T& a, const T& b) { return a < b; }

	export template<ContainingTrivial TContainer>
	void Sort(TContainer& container, Function<bool(typename TContainer::TData, typename TContainer::TData)>&& comparator = DefaultTrivialComparator<typename TContainer::TData>)
	{
		QuickSort(container, 0, container.Size() - 1, Move(comparator));
	}

	export template<ContainingNonTrivial TContainer>
	void Sort(TContainer& container, Function<bool(const typename TContainer::TData&, const typename TContainer::TData&)>&& comparator = DefaultNonTrivialComparator<typename TContainer::TData>)
	{
		QuickSort(container, 0, container.Size() - 1, Move(comparator));
	}
}