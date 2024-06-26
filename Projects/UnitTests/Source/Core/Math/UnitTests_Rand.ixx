// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Rand;

import jpt.Rand;
import jpt.Utilities;

bool UnitTest_GenerateRandomNumbers()
{
	for (size_t i = 0; i < 10; ++i)
	{
		int num = jpt::RNG::Global().RangedInt(-500, 500);
		JPT_ENSURE(num >= -500 && num <= 500);

		num = jpt::RNG::Global().MaxInt(500);
		JPT_ENSURE(num <= 500);

		float f = jpt::RNG::Global().RangedFloat(-50.0f, 100.0f);
		JPT_ENSURE(f >= -50.0f && f <= 100.0f);
		
		bool b = jpt::RNG::Global().Bool();
		JPT_IGNORE(b);
	}

	return true;
}

bool UnitTest_FixedSeedGenerateRandomNumbers()
{
	jpt::RNG rng(990726);

	for (size_t i = 0; i < 10; ++i)
	{
		int num = rng.RangedInt(-500, 500);
		JPT_ENSURE(num >= -500 && num <= 500);

		num = rng.MaxInt(500);
		JPT_ENSURE(num <= 500);

		float f = rng.RangedFloat(-50.0f, 100.0f);
		JPT_ENSURE(f >= -50.0f && f <= 100.0f);

		bool b = rng.Bool();
		JPT_IGNORE(b);
	}

	return true;
}

export bool RunUnitTests_Rand()
{
	JPT_ENSURE(UnitTest_GenerateRandomNumbers());
	JPT_ENSURE(UnitTest_FixedSeedGenerateRandomNumbers());

	return true;
}