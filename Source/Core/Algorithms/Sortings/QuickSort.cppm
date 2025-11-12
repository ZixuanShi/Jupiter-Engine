// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.QuickSort;

import jpt.Concepts;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.Utilities;

export namespace jpt
{
    template<Indexable TContainer, typename TComparator>
    constexpr Index GetPivot(TContainer& container, Index beginIndex, Index endIndex, TComparator&& comparator)
    {
        // Mid of three pivot selection

        // Get each elements
        const auto begin = container[beginIndex];
        const auto mid = container[(beginIndex + endIndex) / 2];
        const auto end = container[endIndex];

        // Compare each elements
        if (comparator(begin, mid))
        {
            if (comparator(mid, end))
            {
                return (beginIndex + endIndex) / 2;
            }
            else if (comparator(begin, end))
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
            if (comparator(begin, end))
            {
                return beginIndex;
            }
            else if (comparator(mid, end))
            {
                return endIndex;
            }
            else
            {
                return (beginIndex + endIndex) / 2;
            }
        }
    }

    template<Indexable TContainer, typename TComparator>
    constexpr Index Partition(TContainer& container, Index beginIndex, Index endIndex, TComparator&& comparator)
    {
        Index pivotIndex = GetPivot(container, beginIndex, endIndex, comparator);
        Swap(container[pivotIndex], container[endIndex]);

        const auto pivot = container[endIndex];
        Index i = beginIndex - 1;    // i is the last element's index of region 1, which is less than the pivot

        // j is the current processing element's index
        for (Index j = beginIndex; j < endIndex; ++j)
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
    constexpr void QuickSort(TContainer& container, Index beginIndex, Index endIndex, TComparator&& comparator)
    {
        // Bounds check
        if (beginIndex >= endIndex || endIndex == kMax<Index>)
        {
            return;
        }

        // Recursively sort the left and right partitions
        const Index pivot = Partition(container, beginIndex, endIndex, Move(comparator));
        QuickSort(container, beginIndex, pivot - 1, Move(comparator));
        QuickSort(container, pivot + 1, endIndex, Move(comparator));
    }
}