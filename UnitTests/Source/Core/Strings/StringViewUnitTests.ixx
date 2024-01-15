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

	strView = pName;
	JPT_RETURN_FALSE_IF_ERROR(strView != pName, "");

	strView = "Hi I am a StringView, Ha";
	JPT_RETURN_FALSE_IF_ERROR(strView != "Hi I am a StringView, Ha", "");

	strView.SubStr(10, 10);
	JPT_RETURN_FALSE_IF_ERROR(strView.SubStr(10, 10) != "StringView", "");

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

	JPT_LOG(strView.substr(10, 10).data());
	JPT_RETURN_FALSE_IF_ERROR(strView.substr(10, 10) != "StringView", "");

	return true;
}

export bool RunStringViewUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTests_StringView(), "UnitTests_StringView Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTests_string_view(), "UnitTests_StringView Failed");

	return true;
}