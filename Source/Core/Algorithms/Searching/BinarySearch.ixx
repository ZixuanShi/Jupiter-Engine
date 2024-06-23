// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.BinarySearch;

import jpt.Concepts;
import jpt.Comparators;
import jpt.Utilities;

export namespace jpt
{
	template<Iterable TContainer, typename TData, typename TComparator = Comparator_Less<void>>
	constexpr auto BinarySearch(const TContainer& container, const TData& dataToFind, TComparator&& comparator = TComparator()) -> decltype(container.begin())
	{
		auto begin = container.begin();
		auto end = container.end();

		while (begin < end)
		{
			auto mid = begin + (end - begin) / 2;

			if (dataToFind == *mid)
			{
				return mid;
			}
			else if (comparator(dataToFind, *mid))
			{
				end = mid;
			}
			else
			{
				begin = mid + 1;
			}
		}

		return container.end();
	}
}