// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module UtilitiesUnitTests;

import jpt.CoreModules;

bool UnitTest_Move()
{
	// String
	jpt::String str1 = "Jupiter";
	jpt::String str2 = "Engine";
	str1 = jpt::Move(str2);
	JPT_RETURN_FALSE_IF_ERROR(str1 != "Engine", "");
	JPT_RETURN_FALSE_IF_ERROR(!str2.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_ERROR(str2.ConstBuffer(), "");

	return true;
}

export bool RunUtilitiesUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Move(), "UnitTest_Move Failed");

	return true;
}