// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Pair;

import jpt.Pair;
import jpt.Utilities;

bool UnitTest_Pair()
{
	jpt::Pair<int, bool> pair{ 42, true };
	//JPT_LOG(pair);

	JPT_ENSURE(pair.first == 42);
	JPT_ENSURE(pair.second == true);

	jpt::Pair<int, bool> otherPair{ 1, false };
	pair = otherPair;
	JPT_ENSURE(pair.first == 1);
	JPT_ENSURE(pair.second == false);
	JPT_ENSURE(pair == otherPair);

	return true;
}

export bool RunUnitTests_Pair()
{
	JPT_ENSURE(UnitTest_Pair());

	return true;
}