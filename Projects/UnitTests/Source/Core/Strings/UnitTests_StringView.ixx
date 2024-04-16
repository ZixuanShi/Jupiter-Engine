// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <string_view>

export module UnitTests_StringView;

import jpt.String;
import jpt.StringView;
import jpt.Utilities;

bool UnitTests_StringView()
{
	jpt::String str = "Jupiter Engine";
	const char* pName = "Zixuan Shi";
	
	jpt::StringView strView(str);
	JPT_ENSURE(strView == str);
	
	jpt::String str2 = "Jupiter";
	jpt::String str3 = "Jupiter Engine Release";
	JPT_ENSURE(strView != str2);
	JPT_ENSURE(strView != str3);

	strView = pName;
	JPT_ENSURE(strView == pName);
	JPT_ENSURE(strView != "Zixuan Shi Core Engineer");

	strView = "Hi I am a StringView, Ha";
	JPT_ENSURE(strView == "Hi I am a StringView, Ha");

	jpt::StringView subStr = strView.SubStr(10, 10);
	JPT_ENSURE(subStr == "StringView");

	JPT_ENSURE(subStr.StartsWith("Str"));
	JPT_ENSURE(!subStr.StartsWith("StrI"));
	JPT_ENSURE(strView.StartsWith("Hi I am"));
	JPT_ENSURE(!strView.StartsWith("Hi am"));

	JPT_ENSURE(subStr.EndsWith("View"));
	JPT_ENSURE(!subStr.EndsWith("ViewwA"));
	JPT_ENSURE(strView.EndsWith("Ha"));
	JPT_ENSURE(!strView.EndsWith("Hi"));

	JPT_ENSURE(strView.Find("StringView") == 10);
	JPT_ENSURE(strView.Find('I') == 3);
	JPT_ENSURE(strView.Find("NoExist") == jpt::npos);
	JPT_ENSURE(strView.Find('N') == jpt::npos);
	JPT_ENSURE(strView.Contains("StringView"));
	JPT_ENSURE(strView.Contains('I'));
	JPT_ENSURE(!strView.Contains("N"));

	return true;
}

export bool RunUnitTests_StringView()
{
	JPT_ENSURE(UnitTests_StringView());

	return true;
}