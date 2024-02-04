// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Searching;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Concepts;

export namespace jpt
{
#pragma region Find
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

	// Refactor with callback

	//template<Iterable TContainer, typename TFinderFunction>
	//constexpr auto Find(TContainer& container, TFinderFunction&& finderFunc) -> decltype(container.begin())
	//{
	//	for (auto it = container.begin(); it != container.end(); ++it)
	//	{
	//		if (finderFunc(*it))
	//		{
	//			return it;
	//		}
	//	}

	//	return container.end();
	//}

	//template<Iterable TContainer, typename TData>
	//constexpr auto Find(const TContainer& container, const TData& dataToFind) -> decltype(container.cbegin())
	//{
	//	for (auto it = container.cbegin(); it != container.cend(); ++it)
	//	{
	//		if (*it == dataToFind)
	//		{
	//			return it;
	//		}
	//	}

	//	return container.cend();
	//}

	//template<Iterable TContainer, typename TFinderFunction>
	//constexpr auto Find(const TContainer& container, TFinderFunction&& finderFunc) -> decltype(container.cbegin())
	//{
	//	for (auto it = container.cbegin(); it != container.cend(); ++it)
	//	{
	//		if (finderFunc(*it))
	//		{
	//			return it;
	//		}
	//	}

	//	return container.cend();
	//}

#pragma endregion

	template<Iterable TContainer, typename TData>
	constexpr bool Contains(TContainer& container, const TData& dataToFind)
	{
		return Find(container, dataToFind) != container.end();
	}
}