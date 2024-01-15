// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

#include <string_view>

export module StringViewUnitTests;

import jpt.CoreModules;

bool UnitTests_StringView()
{
	jpt::String str = "Jupiter Engine";
	const char* pName = "Zixuan Shi";
	
	jpt::StringView strView(str);
	JPT_RETURN_FALSE_IF_ERROR(strView != str, "");
	
	jpt::String str2 = "Jupiter";
	jpt::String str3 = "Jupiter Engine Release";
	JPT_RETURN_FALSE_IF_ERROR(strView == str2, "");
	JPT_RETURN_FALSE_IF_ERROR(strView == str3, "");

	strView = pName;
	JPT_RETURN_FALSE_IF_ERROR(strView != pName, "");
	JPT_RETURN_FALSE_IF_ERROR(strView == "Zixuan Shi Core Engineer", "");

	strView = "Hi I am a StringView, Ha";
	JPT_RETURN_FALSE_IF_ERROR(strView != "Hi I am a StringView, Ha", "");

	jpt::StringView subStr = strView.SubStr(10, 10);
	JPT_RETURN_FALSE_IF_ERROR(subStr != "StringView", "");

	JPT_RETURN_FALSE_IF_ERROR(!subStr.StartsWith("Str"), "");
	JPT_RETURN_FALSE_IF_ERROR(subStr.StartsWith("StrI"), "");
	JPT_RETURN_FALSE_IF_ERROR(!strView.StartsWith("Hi I am"), "");
	JPT_RETURN_FALSE_IF_ERROR(strView.StartsWith("Hi am"), "");

	JPT_RETURN_FALSE_IF_ERROR(!subStr.EndsWith("View"), "");
	JPT_RETURN_FALSE_IF_ERROR(subStr.EndsWith("ViewwA"), "");
	JPT_RETURN_FALSE_IF_ERROR(!strView.EndsWith("Ha"), "");
	JPT_RETURN_FALSE_IF_ERROR(strView.EndsWith("Hi"), "");

	return true;
}

bool UnitTests_string_view()
{
	std::string str = "Jupiter Engine";
	const char* pName = "Zixuan Shi";

	std::string_view strView(str);
	JPT_RETURN_FALSE_IF_ERROR(strView != str, "");

	strView = pName;
	JPT_RETURN_FALSE_IF_ERROR(strView != pName, "");

	strView = "Hi I am a StringView, Ha";
	JPT_RETURN_FALSE_IF_ERROR(strView != "Hi I am a StringView, Ha", "");

	JPT_RETURN_FALSE_IF_ERROR(strView.substr(10, 10) != "StringView", "");

	return true;
}

export bool RunStringViewUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTests_StringView(), "UnitTests_StringView Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTests_string_view(), "UnitTests_StringView Failed");

	return true;
}