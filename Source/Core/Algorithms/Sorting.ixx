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
	template<typename T>
	bool DefaultComparator(const T& a, const T& b)
	{
		JPT_LOG("Using default comparator");
		return a < b;
	}
}

namespace jpt
{
	export template<Iterable TContainer>
	void Sort(TContainer& container, Function<bool(const typename TContainer::TData&, const typename TContainer::TData&)>&& comparator = DefaultComparator<typename TContainer::TData>)
	{
		comparator(1, 2);
		JPT_IGNORE(container, comparator);
	}
}