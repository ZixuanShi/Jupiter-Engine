// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.IntroSort;

import jpt.Concepts;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.InsertionSort;
import jpt.QuickSort;
import jpt.HeapSort;
import jpt.Utilities;

namespace jpt
{
    template<Indexable TContainer, typename TComparator>
    constexpr void IntroSortDepth(TContainer& container, size_t beginIndex, size_t endIndex, size_t depth, TComparator&& comparator)
    {
        static constexpr size_t kInsertionSortThreshold = 16;

        if (beginIndex >= endIndex || endIndex == Constants<size_t>::kMax)
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
            IntroSortDepth(container, beginIndex, pivot - 1, depth - 1, Move(comparator));
            IntroSortDepth(container, pivot + 1, endIndex, depth - 1, Move(comparator));
        }
    }

    export template<Indexable TContainer, typename TComparator>
    constexpr void IntroSort(TContainer& container, size_t beginIndex, size_t endIndex, TComparator&& comparator)
    {
        const size_t depth = static_cast<size_t>(std::log2(endIndex - beginIndex + 1) * 2);
        IntroSortDepth(container, beginIndex, endIndex, depth, Move(comparator));
    }
}