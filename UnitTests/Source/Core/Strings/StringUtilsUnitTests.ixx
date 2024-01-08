// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

export module StringUtilsUnitTests;

import jpt.CoreModules;

template<class StringT>
bool UnitTest_StringLen()
{
	using CharT = StringT::CharT;

	JPT_RETURN_FALSE_IF_ERROR(jpt::GetCStrLength(JPT_GET_PROPER_STRING(CharT, Jupiter Engine)) != 14, "");
	JPT_RETURN_FALSE_IF_ERROR(jpt::GetCStrLength(JPT_GET_PROPER_STRING(CharT, Zixuan Shi)) != 10, "");
	return true;
}

template<class StringT>
bool UnitTest_ToCStr()
{
	using CharT = StringT::CharT;

	const CharT* integerCStr = jpt::IntegerToCStr<CharT>(114514);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(integerCStr, JPT_GET_PROPER_STRING(CharT, 114514), 6), "");
	delete integerCStr;

	integerCStr = jpt::IntegerToCStr<CharT>(-114514);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(integerCStr, JPT_GET_PROPER_STRING(CharT, -114514), 7), "");
	delete integerCStr;

	int32 num = jpt::CStrToInteger<CharT>(JPT_GET_PROPER_STRING(CharT, 114514), 6);
	JPT_RETURN_FALSE_IF_ERROR(num != 114514, "");

	num = jpt::CStrToInteger<CharT>(JPT_GET_PROPER_STRING(CharT, -114514), 7);
	JPT_RETURN_FALSE_IF_ERROR(num != -114514, "");

	float f = jpt::CStrToFloat<CharT>(JPT_GET_PROPER_STRING(CharT, 114514.114514), 13);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreValuesClose(f, 114514.114514f), "");

	f = jpt::CStrToFloat<CharT>(JPT_GET_PROPER_STRING(CharT, -114514.114514), 14);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreValuesClose(f, -114514.114514f), "");

	const CharT* floatingCStr = jpt::FloatToCStr<CharT>(-114514.114f);
	//JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(floatingCStr, "-114514.114", 11), "");	// Not stable
	delete floatingCStr;

	return true;
}

template<class StringT>
bool UnitTest_StrCpy()
{
	using CharT = StringT::CharT;

	CharT buffer[256];
	jpt::StrCpy<CharT>(buffer, 15, JPT_GET_PROPER_STRING(CharT, Jupiter Engine));
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(buffer, JPT_GET_PROPER_STRING(CharT, Jupiter Engine), 14), "");

	jpt::StrNCpy<CharT>(buffer, 15, JPT_GET_PROPER_STRING(CharT, Jupiter Engine), 10);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(buffer, JPT_GET_PROPER_STRING(CharT, Jupiter En), 10), "");

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