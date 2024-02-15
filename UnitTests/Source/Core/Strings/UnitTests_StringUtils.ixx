// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_StringUtils;

import jpt.String;
import jpt.StringUtils;
import jpt.Math;

template<class TString>
bool UnitTest_StringLen()
{
	using TChar = TString::TChar;

	JPT_RETURN_FALSE_IF_ERROR(jpt::GetCStrLength(JPT_GET_PROPER_STRING(TChar, Jupiter Engine)) != 14, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetCStrLength(JPT_GET_PROPER_STRING(TChar, Zixuan Shi)) != 10, "");
	return true;
}

template<class TString>
bool UnitTest_ToCStr()
{
	using TChar = TString::TChar;

	const TChar* integerCStr = jpt::IntegerToCStr<TChar>(114514);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(integerCStr, JPT_GET_PROPER_STRING(TChar, 114514), 6), "");
	delete integerCStr;

	integerCStr = jpt::IntegerToCStr<TChar>(-114514);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(integerCStr, JPT_GET_PROPER_STRING(TChar, -114514), 7), "");
	delete integerCStr;

	int32 num = jpt::CStrToInteger<TChar>(JPT_GET_PROPER_STRING(TChar, 114514), 6);
	JPT_RETURN_FALSE_IF_ERROR(num != 114514, "");

	num = jpt::CStrToInteger<TChar>(JPT_GET_PROPER_STRING(TChar, -114514), 7);
	JPT_RETURN_FALSE_IF_ERROR(num != -114514, "");

	float f = jpt::CStrToFloat<TChar>(JPT_GET_PROPER_STRING(TChar, 114514.114514), 13);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreValuesClose(f, 114514.114514f), "");

	f = jpt::CStrToFloat<TChar>(JPT_GET_PROPER_STRING(TChar, -114514.114514), 14);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreValuesClose(f, -114514.114514f), "");

	const TChar* floatingCStr = jpt::FloatToCStr<TChar>(-114514.114f);
	//JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(floatingCStr, "-114514.114", 11), "");	// Not stable
	delete floatingCStr;

	return true;
}

template<class TString>
bool UnitTest_StrCpy()
{
	using TChar = TString::TChar;

	TChar buffer[256];
	jpt::StrCpy<TChar>(buffer, 15, JPT_GET_PROPER_STRING(TChar, Jupiter Engine));
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(buffer, JPT_GET_PROPER_STRING(TChar, Jupiter Engine)), "");

	jpt::StrNCpy<TChar>(buffer, 15, JPT_GET_PROPER_STRING(TChar, Jupiter Engine), 10);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(buffer, JPT_GET_PROPER_STRING(TChar, Jupiter En)), "");

	return true;
}

bool RunUnitTests_IsValidDataCStr()
{
	// Empty
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsValidDataCStr(""), "");

	// number
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsValidDataCStr("1"), "");
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsValidDataCStr("1010"), "");
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsValidDataCStr("9876543210"), "");

	// text
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsValidDataCStr("JupiterEngine"), "");

	// underscore
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsValidDataCStr("_"), "");

	// special
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsValidDataCStr("!"), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::IsValidDataCStr("!", false), "");
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsValidDataCStr("!@#$%^&*()_+-=[]\\{}|;':\",./<>?"), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::IsValidDataCStr("!@#$%^&*()_+-=[]\\{}|;':\",./<>?", false), "");

	// Combination
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsValidDataCStr("Jupiter-Engine!@#$%^&*()_+-=[]\\{}|;':\",./<>?"), "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::IsValidDataCStr("Jupiter_Engine!@#$%^&*()_+-=[]\\{}|;':\",./<>?", false), "");
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsValidDataCStr("Jupiter_Engine"), "");
	JPT_RETURN_FALSE_IF_ERROR(!jpt::IsValidDataCStr("_Jupiter_Engine", false), "");

	return true;
}

export bool RunUnitTests_StringUtils()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StringLen<jpt::String>(), "UnitTest_StringLen Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StringLen<jpt::WString>(), "UnitTest_StringLen Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_ToCStr<jpt::String>(), "UnitTest_ToCStr Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_ToCStr<jpt::WString>(), "UnitTest_ToCStr Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StrCpy<jpt::String>(), "UnitTest_ToCStr Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StrCpy<jpt::WString>(), "UnitTest_ToCStr Failed");

	JPT_RETURN_FALSE_IF_ERROR(!RunUnitTests_IsValidDataCStr(), "RunUnitTests_IsValidDataCStr Failed");

	return true;
}