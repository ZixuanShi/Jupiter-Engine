// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_RandomNumberGenerator;

import jpt.RandomNumberGenerator;

bool UnitTest_GenerateRandomNumbers()
{
	for (size_t i = 0; i < 10; ++i)
	{
		int num = jpt::RNG::Global().RandInRange(-500, 500);
		JPT_RETURN_FALSE_IF_ERROR(num < -500 || num > 500, "");

		num = jpt::RNG::Global().RandInMax(500);
		JPT_RETURN_FALSE_IF_ERROR(num > 500, "");

		float f = jpt::RNG::Global().FRand(-50.0f, 100.0f);
		JPT_RETURN_FALSE_IF_ERROR(f < -50.0f || f > 100.0f, "");
		
		bool b = jpt::RNG::Global().RandBool();
		JPT_IGNORE(b);
	}

	return true;
}

bool UnitTest_FixedSeedGenerateRandomNumbers()
{
	jpt::RNG rng(990726);

	for (size_t i = 0; i < 10; ++i)
	{
		int num = rng.RandInRange(-500, 500);
		JPT_RETURN_FALSE_IF_ERROR(num < -500 || num > 500, "");

		num = rng.RandInMax(500);
		JPT_RETURN_FALSE_IF_ERROR(num > 500, "");

		float f = rng.FRand(-50.0f, 100.0f);
		JPT_RETURN_FALSE_IF_ERROR(f < -50.0f || f > 100.0f, "");

		bool b = rng.RandBool();
		JPT_IGNORE(b);
	}

	return true;
}

export bool RunUnitTests_RandomNumberGenerator()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_GenerateRandomNumbers(), "UnitTest_GenerateRandomNumbers Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_FixedSeedGenerateRandomNumbers(), "UnitTest_FixedSeedGenerateRandomNumbers Failed");

	return true;
}