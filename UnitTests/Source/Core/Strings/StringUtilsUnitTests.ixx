// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

export module StringUtilsUnitTests;

import jpt.CoreModules;

template<class StringType>
bool UnitTest_StringLen()
{
	using CharType = StringType::CharType;

	JPT_RETURN_FALSE_IF_ERROR(jpt::GetCStrLength(JPT_GET_PROPER_STRING(CharType, Jupiter Engine)) != 14, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetCStrLength(JPT_GET_PROPER_STRING(CharType, Zixuan Shi)) != 10, "");
	return true;
}

template<class StringType>
bool UnitTest_ToCStr()
{
	using CharType = StringType::CharType;

	const CharType* integerCStr = jpt::IntegerToCStr<CharType>(114514);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(integerCStr, JPT_GET_PROPER_STRING(CharType, 114514), 6), "");
	delete integerCStr;

	integerCStr = jpt::IntegerToCStr<CharType>(-114514);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(integerCStr, JPT_GET_PROPER_STRING(CharType, -114514), 7), "");
	delete integerCStr;

	int32 num = jpt::CStrToInteger<CharType>(JPT_GET_PROPER_STRING(CharType, 114514), 6);
	JPT_RETURN_FALSE_IF_ERROR(num != 114514, "");

	num = jpt::CStrToInteger<CharType>(JPT_GET_PROPER_STRING(CharType, -114514), 7);
	JPT_RETURN_FALSE_IF_ERROR(num != -114514, "");

	float f = jpt::CStrToFloat<CharType>(JPT_GET_PROPER_STRING(CharType, 114514.114514), 13);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreValuesClose(f, 114514.114514f), "");

	f = jpt::CStrToFloat<CharType>(JPT_GET_PROPER_STRING(CharType, -114514.114514), 14);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreValuesClose(f, -114514.114514f), "");

	const CharType* floatingCStr = jpt::FloatToCStr<CharType>(-114514.114f);
	//JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(floatingCStr, "-114514.114", 11), "");	// Not stable
	delete floatingCStr;

	return true;
}

template<class StringType>
bool UnitTest_StrCpy()
{
	using CharType = StringType::CharType;

	CharType buffer[256];
	jpt::StrCpy<CharType>(buffer, 15, JPT_GET_PROPER_STRING(CharType, Jupiter Engine));
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(buffer, JPT_GET_PROPER_STRING(CharType, Jupiter Engine), 14), "");

	jpt::StrNCpy<CharType>(buffer, 15, JPT_GET_PROPER_STRING(CharType, Jupiter Engine), 10);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(buffer, JPT_GET_PROPER_STRING(CharType, Jupiter En), 10), "");

	return true;
}

export bool RunStringUtilsUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StringLen<jpt::String>(), "UnitTest_StringLen Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StringLen<jpt::WString>(), "UnitTest_StringLen Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_ToCStr<jpt::String>(), "UnitTest_ToCStr Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_ToCStr<jpt::WString>(), "UnitTest_ToCStr Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StrCpy<jpt::String>(), "UnitTest_ToCStr Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StrCpy<jpt::WString>(), "UnitTest_ToCStr Failed");

	return true;
}