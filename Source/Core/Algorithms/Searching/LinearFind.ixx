// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"

export module jpt.LinearFind;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Concepts;
import jpt.Constants;

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

	template<Iterable TContainer, typename TFinderFunction>
	constexpr auto FindIf(TContainer& container, TFinderFunction&& finderFunc) -> decltype(container.begin())
	{
		for (auto it = container.begin(); it != container.end(); ++it)
		{
			if (finderFunc(*it))
			{
				return it;
			}
		}

		return container.end();
	}

	template<Iterable TContainer, typename TData>
	constexpr auto Find(const TContainer& container, const TData& dataToFind) -> decltype(container.cbegin())
	{
		for (auto it = container.cbegin(); it != container.cend(); ++it)
		{
			if (*it == dataToFind)
			{
				return it;
			}
		}

		return container.cend();
	}

	template<Iterable TContainer, typename TFinderFunction>
	constexpr auto FindIf(const TContainer& container, TFinderFunction&& finderFunc) -> decltype(container.cbegin())
	{
		for (auto it = container.cbegin(); it != container.cend(); ++it)
		{
			if (finderFunc(*it))
			{
				return it;
			}
		}

		return container.cend();
	}

	template<typename TContainer, typename TData>
	constexpr size_t Find(const TContainer& pArray, const TData& dataToFind)
	{
		for (size_t i = 0; i < JPT_ARRAY_COUNT(pArray); ++i)
		{
			if (pArray[i] == dataToFind)
			{
				return i;
			}
		}

		return kInvalidValue<size_t>;
	}

#pragma endregion Find

#pragma region Has
	template<Iterable TContainer, typename TData>
	constexpr bool Has(TContainer& container, const TData& dataToFind)
	{
		return Find(container, dataToFind) != container.end();
	}

	template<Iterable TContainer, typename TFinderFunction>
	constexpr bool HasIf(TContainer& container, TFinderFunction&& finderFunc)
	{
		return FindIf(container, Forward<TFinderFunction>(finderFunc)) != container.end();
	}

#pragma endregion Has
}