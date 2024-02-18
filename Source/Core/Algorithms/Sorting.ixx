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

namespace jpt
{
	template<Trivial T>
	bool DefaultComparator(T a, T b)
	{
		JPT_LOG("Using NonTrivial comparator");
		return a < b;
	}
}

namespace jpt
{
	export template<ContainingTrivial TContainer>
	void Sort(TContainer& container, Function<bool(typename TContainer::TData, typename TContainer::TData)>&& comparator = DefaultComparator<typename TContainer::TData>)
	{
		comparator(1, 2);
		JPT_IGNORE(container, comparator);
	}
}