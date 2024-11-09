// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Debugging/Logger.h"

export module UnitTests_SortedSet;

import jpt.SortedSet;
import jpt.TypeDefs;
import jpt.Rand;
import jpt.Constants;
import jpt.Comparators;

jpt::RNG rng(1);

static bool Add_Less()
{
	jpt::SortedSet<int32> sortedSet;
	for (int32 i = 0; i < 10; ++i)
	{
		int32 value = rng.MaxInt(100);
		sortedSet.Add(value);
	}

	JPT_ENSURE(sortedSet.Count() == 10);

	int32 previous = jpt::kInvalidValue<int32>;
	for (auto itr = sortedSet.begin(); itr != sortedSet.end(); ++itr)
	{
		if (previous == jpt::kInvalidValue<int32>)
		{
			previous = *itr;
		}
		else
		{
			JPT_ENSURE(previous <= *itr);
			previous = *itr;
		}
	}

	return true;
}

static bool Add_Greater()
{
	jpt::SortedSet<int32, jpt::Comparator_Greater<int32>> sortedSet;
	for (int32 i = 0; i < 10; ++i)
	{
		int32 value = rng.MaxInt(100);
		sortedSet.Add(value);
	}

	JPT_ENSURE(sortedSet.Count() == 10);

	int32 previous = jpt::kInvalidValue<int32>;
	for (auto itr = sortedSet.begin(); itr != sortedSet.end(); ++itr)
	{
		if (previous == jpt::kInvalidValue<int32>)
		{
			previous = *itr;
		}
		else
		{
			JPT_ENSURE(previous >= *itr);
			previous = *itr;
		}
	}

	return true;
}

export bool RunUnitTests_SortedSet()
{
	JPT_ENSURE(Add_Less());
	JPT_ENSURE(Add_Greater());

	return true;
}