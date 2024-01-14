// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

#include <string_view>

export module StringViewUnitTests;

import jpt.CoreModules;

bool UnitTests_StringView()
{
	jpt::String str = "Jupiter Engine";
	//jpt::StringView strView(str);

	return true;
}

bool UnitTests_string_view()
{
	std::string str = "Jupiter Engine";
	std::string_view strView(str);
	strView = "Hi";

	return true;
}

export bool RunStringViewUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTests_StringView(), "UnitTests_StringView Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTests_string_view(), "UnitTests_StringView Failed");

	return true;
}