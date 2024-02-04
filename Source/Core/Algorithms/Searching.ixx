// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Searching;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Concepts;

export namespace jpt
{
	template<Iterable TContainer, typename TData>
	constexpr auto Find(TContainer& container, const TData& dataToFind) -> decltype(container.begin())
	{
		for(auto it = container.begin(); it != container.end(); ++it)
		{
			if (*it == dataToFind)
			{
				return it;
			}
		}

		return container.end();
	}

	template<Iterable TContainer, typename TData>
	constexpr bool Contains(TContainer& container, const TData& dataToFind)
	{
		return Find(container, dataToFind) != container.end();
	}
}