// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module UnitTests_Optional;

import jpt.CoreModules;

bool UnitTest_Optional()
{
	jpt::Optional<int32> intOptional;
	JPT_RETURN_FALSE_IF_ERROR(intOptional.HasValue(), "");

	intOptional = 42;
	JPT_RETURN_FALSE_IF_ERROR(!intOptional.HasValue(), "");
	JPT_RETURN_FALSE_IF_ERROR(intOptional.Value() != 42, "");

	int32 num = 12;
	intOptional = num;
	JPT_RETURN_FALSE_IF_ERROR(intOptional.Value() != 12, "");

	intOptional.Reset();
	JPT_RETURN_FALSE_IF_ERROR(intOptional.HasValue(), "");

	jpt::Optional<jpt::String> stringOptional;
	JPT_RETURN_FALSE_IF_ERROR(stringOptional.HasValue(), "");

	stringOptional = "42";
	JPT_RETURN_FALSE_IF_ERROR(!stringOptional.HasValue(), "");
	JPT_RETURN_FALSE_IF_ERROR(stringOptional.Value() != "42", "");

	jpt::String str("Jupiter Engine");
	stringOptional = jpt::Move(str);
	JPT_RETURN_FALSE_IF_ERROR(stringOptional.Value() != "Jupiter Engine", "");

	stringOptional.Reset();
	JPT_RETURN_FALSE_IF_ERROR(stringOptional.HasValue(), "");

	return true;
}

export bool RunUnitTests_Optional()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Optional(), "UnitTest_Optional Failed");

	return true;
}