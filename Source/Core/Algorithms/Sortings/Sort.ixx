// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"

export module jpt.Sort;

import jpt.Comparators;
import jpt.Concepts;
import jpt.Utilities;
import jpt.IntroSort;

export namespace jpt
{
    // Jupiter's containers with Count() function
    template<typename TContainer, typename TComparator = Comparator_Less<void>> requires Countable<TContainer> && Indexable<TContainer>
    constexpr void Sort(TContainer& container, TComparator&& comparator = TComparator())
    {
        IntroSort(container, 0, container.Count() - 1, Move(comparator));
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