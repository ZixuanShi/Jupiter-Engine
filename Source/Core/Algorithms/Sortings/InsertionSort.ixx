// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.InsertionSort;

import jpt.Concepts;
import jpt.TypeDefs;

export namespace jpt
{
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
}