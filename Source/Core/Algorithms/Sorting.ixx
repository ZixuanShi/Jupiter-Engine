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
	template<typename T>
	constexpr size_t GetPivot(T* pBuffer, size_t beginIndex, size_t endIndex)
	{
		// Mid of three pivot selection

		// Get each elements
		const T first = pBuffer[beginIndex];
		const T mid   = pBuffer[endIndex / 2];
		const T last  = pBuffer[beginIndex];

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

	template<typename T, typename TComparator>
	constexpr size_t Partition(T* pBuffer, size_t beginIndex, size_t endIndex, TComparator&& comparator)
	{
		size_t pivotIndex = GetPivot(pBuffer, beginIndex, endIndex);
		Swap(pBuffer[pivotIndex], pBuffer[endIndex]);

		T pivot = pBuffer[endIndex];
		size_t i = beginIndex - 1;	// i is the last element's index of region 1, which is less than the pivot

		// j is the current processing element's index
		for (size_t j = beginIndex; j < endIndex; ++j)
		{
			// if current element should be placed to region one
			// Swap last element in region 1 with current processing element.
			if (comparator(pBuffer[j], pivot))
			{
				++i;
				Swap(pBuffer[i], pBuffer[j]);
			}
		}

		// Everything is in its place except for the pivot. We swap the pivot with the first element of region 2.
		pivotIndex = i + 1;
		Swap(pBuffer[pivotIndex], pBuffer[endIndex]);

		// return the pivot, which becomes the beginning and end points of the next calls to Partition().
		return pivotIndex;
	}

	template<typename T, typename TComparator>
	constexpr void QuickSort(T* pBuffer, size_t beginIndex, size_t endIndex, TComparator&& comparator)
	{
		// Bounds check
		if (beginIndex >= endIndex || endIndex == LimitsOf<size_t>::kMax)
		{
			return;
		}

		// Recursively sort the left and right partitions
		const size_t pivot = Partition(pBuffer, beginIndex, endIndex, Move(comparator));
		QuickSort(pBuffer, beginIndex, pivot - 1, Move(comparator));
		QuickSort(pBuffer, pivot + 1, endIndex, Move(comparator));
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
		jpt_private::QuickSort(container.Buffer(), 0, container.Size() - 1, Move(comparator));
	}

	export template<ContainingNonTrivial TContainer>
	constexpr void Sort(TContainer& container, Function<bool(const typename TContainer::TData&, const typename TContainer::TData&)>&& comparator = DefaultNonTrivialComparator<typename TContainer::TData>)
	{
		jpt_private::QuickSort(container.Buffer(), 0, container.Size() - 1, Move(comparator));
	}

	export template<Trivial T>
	constexpr void Sort(T* pBuffer, size_t size, Function<bool(T, T)>&& comparator = DefaultTrivialComparator<T>)
	{
		jpt_private::QuickSort(pBuffer, 0, size - 1, Move(comparator));
	}

	export template<NonTrivial T>
	constexpr void Sort(T* pBuffer, size_t size, Function<bool(const T&, const T&)>&& comparator = DefaultNonTrivialComparator<T>)
	{
		jpt_private::QuickSort(pBuffer, 0, size - 1, Move(comparator));
	}
}