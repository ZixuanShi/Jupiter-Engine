// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Optional;

import jpt.TypeDefs;
import jpt.Utilities;
import jpt.Optional;
import jpt.String;

bool UnitTest_Optional()
{
	jpt::Optional<int32> intOptional;
	JPT_ENSURE(!intOptional.HasValue());

	intOptional = 42;
	JPT_ENSURE(intOptional.HasValue());
	JPT_ENSURE(intOptional.Value() == 42);

	int32 num = 12;
	intOptional = num;
	JPT_ENSURE(intOptional.Value() == 12);

	intOptional.Reset();
	JPT_ENSURE(!intOptional.HasValue());

	jpt::Optional<jpt::String> stringOptional;
	JPT_ENSURE(!stringOptional.HasValue());

	stringOptional = "42";
	JPT_ENSURE(stringOptional.HasValue());
	JPT_ENSURE(stringOptional.Value() == "42");

	jpt::String str("Jupiter Engine");
	stringOptional = jpt::Move(str);
	JPT_ENSURE(stringOptional.Value() == "Jupiter Engine");

	stringOptional.Reset();
	JPT_ENSURE(!stringOptional.HasValue());

	return true;
}

export bool RunUnitTests_Optional()
{
	JPT_ENSURE(UnitTest_Optional());

	return true;
}