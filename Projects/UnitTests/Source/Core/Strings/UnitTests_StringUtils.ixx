// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_StringUtils;

import jpt.String;
import jpt.StringUtils;
import jpt.Math;
import jpt.Utilities;

template<class TString>
bool UnitTest_StringLen()
{
	using TChar = TString::TChar;

	JPT_ENSURE(jpt::FindCharsCount(JPT_GET_PROPER_STRING(TChar, Jupiter Engine)) == 14);
	JPT_ENSURE(jpt::FindCharsCount(JPT_GET_PROPER_STRING(TChar, Zixuan Shi)) == 10);
	return true;
}

template<class TString>
bool UnitTest_ToCStr()
{
	using TChar = TString::TChar;

	const TChar* integerCStr = jpt::IntegerToCStr<TChar>(114514);
	JPT_ENSURE(jpt::AreStringsSame(integerCStr, JPT_GET_PROPER_STRING(TChar, 114514), 6));
	delete integerCStr;

	integerCStr = jpt::IntegerToCStr<TChar>(-114514);
	JPT_ENSURE(jpt::AreStringsSame(integerCStr, JPT_GET_PROPER_STRING(TChar, -114514), 7));
	delete integerCStr;

	int32 num = jpt::CStrToInteger<TChar>(JPT_GET_PROPER_STRING(TChar, 114514), 6);
	JPT_ENSURE(num == 114514);

	num = jpt::CStrToInteger<TChar>(JPT_GET_PROPER_STRING(TChar, -114514), 7);
	JPT_ENSURE(num == -114514);

	float f = jpt::CStrToFloat<TChar>(JPT_GET_PROPER_STRING(TChar, 114514.114514), 13);
	JPT_ENSURE(jpt::AreValuesClose(f, 114514.114514f));

	f = jpt::CStrToFloat<TChar>(JPT_GET_PROPER_STRING(TChar, -114514.114514), 14);
	JPT_ENSURE(jpt::AreValuesClose(f, -114514.114514f));

	const TChar* floatingCStr = jpt::FloatToCStr<TChar>(-114514.114f);
	//JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(floatingCStr, "-114514.114", 11));	// Not stable
	delete floatingCStr;

	return true;
}

template<class TString>
bool UnitTest_StrCpy()
{
	using TChar = TString::TChar;

	TChar buffer[256];
	jpt::StrCpy<TChar>(buffer, 15, JPT_GET_PROPER_STRING(TChar, Jupiter Engine));
	JPT_ENSURE(jpt::AreStringsSame(buffer, JPT_GET_PROPER_STRING(TChar, Jupiter Engine)));

	jpt::StrNCpy<TChar>(buffer, 15, JPT_GET_PROPER_STRING(TChar, Jupiter Engine), 10);
	JPT_ENSURE(jpt::AreStringsSame(buffer, JPT_GET_PROPER_STRING(TChar, Jupiter En)));

	return true;
}

bool RunUnitTests_IsValidDataCStr()
{
	// Empty
	JPT_ENSURE(jpt::IsValidDataCStr(""));

	// number
	JPT_ENSURE(jpt::IsValidDataCStr("1"));
	JPT_ENSURE(jpt::IsValidDataCStr("1010"));
	JPT_ENSURE(jpt::IsValidDataCStr("9876543210"));

	// text
	JPT_ENSURE(jpt::IsValidDataCStr("JupiterEngine"));

	// underscore
	JPT_ENSURE(jpt::IsValidDataCStr("_"));

	// special
	JPT_ENSURE(jpt::IsValidDataCStr("!"));
	JPT_ENSURE(jpt::IsValidDataCStr("!Jupiter"));
	JPT_ENSURE(!jpt::IsValidDataCStr("!", false));
	JPT_ENSURE(jpt::IsValidDataCStr("!@#$%^&*()_+-=[]\\{}|;':\",./<>?"));
	JPT_ENSURE(!jpt::IsValidDataCStr("!@#$%^&*()_+-=[]\\{}|;':\",./<>?", false));

	// Combination
	JPT_ENSURE(jpt::IsValidDataCStr("Jupiter-Engine!@#$%^&*()_+-=[]\\{}|;':\",./<>?"));
	JPT_ENSURE(!jpt::IsValidDataCStr("Jupiter_Engine!@#$%^&*()_+-=[]\\{}|;':\",./<>?", false));
	JPT_ENSURE(jpt::IsValidDataCStr("Jupiter_Engine"));
	JPT_ENSURE(jpt::IsValidDataCStr("_Jupiter_Engine", false));

	return true;
}

bool RunUnitTests_StringUtils_IntegerToCStr()
{
	const char* result = jpt::IntegerToCStr(123456);
	JPT_ENSURE(jpt::AreStringsSame(result, "123456"));
	delete result;

	// Hex
	result = jpt::IntegerToCStr(0xFF00FF00, IntBase::Hex);
	JPT_ENSURE(jpt::AreStringsSame(result, "0xFF00FF00"));
	delete result;

	return true;
}

bool RunUnitTests_StringUtils_CStrToInteger()
{
	// Decimal
	JPT_ENSURE(jpt::CStrToInteger("123456") == 123456);

	// Hex
	uint32 result = jpt::CStrToInteger("0xF", 3, IntBase::Hex);
	JPT_ENSURE(result == 0xF);

	result = jpt::CStrToInteger("0xFF00FFAA", 10, IntBase::Hex);
	JPT_ENSURE(result == 0xFF00FFAA);
	
	return true;
}

export bool RunUnitTests_StringUtils()
{
	JPT_ENSURE(UnitTest_StringLen<jpt::String>());
	JPT_ENSURE(UnitTest_StringLen<jpt::WString>());

	JPT_ENSURE(UnitTest_ToCStr<jpt::String>());
	JPT_ENSURE(UnitTest_ToCStr<jpt::WString>());

	JPT_ENSURE(UnitTest_StrCpy<jpt::String>());
	JPT_ENSURE(UnitTest_StrCpy<jpt::WString>());

	JPT_ENSURE(RunUnitTests_IsValidDataCStr());
	JPT_ENSURE(RunUnitTests_StringUtils_IntegerToCStr());
	JPT_ENSURE(RunUnitTests_StringUtils_CStrToInteger());

	return true;
}