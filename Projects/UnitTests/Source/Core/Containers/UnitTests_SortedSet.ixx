// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module UnitTests_SortedSet;

import jpt.SortedSet;
import jpt.TypeDefs;
import jpt.Rand;

static bool Add()
{
	jpt::SortedSet<int32> sortedSet;
	jpt::RNG& rng = jpt::RNG::Global();
	for (int32 i = 0; i < 10; ++i)
	{
		int32 value = rng.MaxInt(100);
		sortedSet.Add(value);
		JPT_LOG(value);
	}

	JPT_LOG(sortedSet);

	return true;
}

export bool RunUnitTests_SortedSet()
{
	JPT_ENSURE(Add());

	return true;
}