// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

export module RandomNumberGeneratorUnitTests;

import jpt.CoreModules;

bool UnitTest_GenerateRandomNumbers()
{
	for (size_t i = 0; i < 10; ++i)
	{
		int num = jpt::RNG::Global().RandInRange(-500, 500);
		JPT_RETURN_FALSE_IF_LOG(num < -500 || num > 500, "");

		num = jpt::RNG::Global().RandInMax(500);
		JPT_RETURN_FALSE_IF_LOG(num > 500, "");

		float f = jpt::RNG::Global().FRand(-50.0f, 100.0f);
		JPT_RETURN_FALSE_IF_LOG(f < -50.0f || f > 100.0f, "");
		
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
		JPT_RETURN_FALSE_IF_LOG(num < -500 || num > 500, "");

		num = rng.RandInMax(500);
		JPT_RETURN_FALSE_IF_LOG(num > 500, "");

		float f = rng.FRand(-50.0f, 100.0f);
		JPT_RETURN_FALSE_IF_LOG(f < -50.0f || f > 100.0f, "");

		bool b = rng.RandBool();
		JPT_IGNORE(b);
	}

	return true;
}

export void RunRandomNumberGeneratorUnitTests()
{
	JPT_LOG_IF(!UnitTest_GenerateRandomNumbers(), "UnitTest_GenerateRandomNumbers Failed");
	JPT_LOG_IF(!UnitTest_FixedSeedGenerateRandomNumbers(), "UnitTest_FixedSeedGenerateRandomNumbers Failed");

}