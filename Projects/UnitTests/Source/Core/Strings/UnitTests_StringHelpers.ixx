// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"
#include "Core/Strings/StringMacros.h"

export module UnitTests_StringUtils;

import jpt.String;
import jpt.String.Helpers;
import jpt.Math;
import jpt.Utilities;

template<class TString>
bool UnitTests_StringLen()
{
	using TChar = TString::TChar;

	JPT_ENSURE(jpt::FindCharsCount(JPT_GET_PROPER_STRING(TChar, Jupiter Engine)) == 14);
	JPT_ENSURE(jpt::FindCharsCount(JPT_GET_PROPER_STRING(TChar, Zixuan Shi)) == 10);
	return true;
}

template<class TString>
bool UnitTests_ToCStr()
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
bool UnitTests_StrCpy()
{
	using TChar = TString::TChar;

	TChar buffer[256];
	jpt::StrCpy<TChar>(buffer, 15, JPT_GET_PROPER_STRING(TChar, Jupiter Engine));
	JPT_ENSURE(jpt::AreStringsSame(buffer, JPT_GET_PROPER_STRING(TChar, Jupiter Engine)));

	jpt::StrNCpy<TChar>(buffer, 15, JPT_GET_PROPER_STRING(TChar, Jupiter Engine), 10);
	JPT_ENSURE(jpt::AreStringsSame(buffer, JPT_GET_PROPER_STRING(TChar, Jupiter En)));

	return true;
}

bool RunUnitTests_StringUtils_IntegerToCStr()
{
	const char* result = jpt::IntegerToCStr(123456);
	JPT_ENSURE(jpt::AreStringsSame(result, "123456"));
	delete result;

	// Hex
	result = jpt::IntegerToCStr(0xFF00FF00, EIntBase::Hex);
	JPT_ENSURE(jpt::AreStringsSame(result, "0xFF00FF00"));
	delete result;

	return true;
}

bool RunUnitTests_StringUtils_CStrToInteger()
{
	// Decimal
	JPT_ENSURE(jpt::CStrToInteger("123456") == 123456);

	// Hex
	uint32 result = jpt::CStrToInteger("0xF", 3, EIntBase::Hex);
	JPT_ENSURE(result == 0xF);

	result = jpt::CStrToInteger("0xFF00FFAA", 10, EIntBase::Hex);
	JPT_ENSURE(result == 0xFF00FFAA);
	
	return true;
}

bool RunUnitTests_StringUtils_CStrToWStr()
{
	wchar_t* wchars = nullptr;
		
	wchars = jpt::ToWChars("");
	JPT_ENSURE(jpt::AreStringsSame(wchars, L""));
	delete[] wchars;

	wchars = jpt::ToWChars("Jupiter Engine");
	JPT_ENSURE(jpt::AreStringsSame(wchars, L"Jupiter Engine"));
	delete[] wchars;

	wchars = jpt::ToWChars("Zixuan Shi");
	JPT_ENSURE(jpt::AreStringsSame(wchars, L"Zixuan Shi"));
	delete[] wchars;

	return true;
}
bool RunUnitTests_StringUtils_WStrToCStr()
{
	char* chars = nullptr;

	chars = jpt::ToChars(L"");
	JPT_ENSURE(jpt::AreStringsSame(chars, ""));
	delete[] chars;

	chars = jpt::ToChars(L"Jupiter Engine");
	JPT_ENSURE(jpt::AreStringsSame(chars, "Jupiter Engine"));
	delete[] chars;

	chars = jpt::ToChars(L"Zixuan Shi");
	JPT_ENSURE(jpt::AreStringsSame(chars, "Zixuan Shi"));
	delete[] chars;

	return true;
}

bool UnitTests_IsSpace()
{
	JPT_ENSURE(' ');
	JPT_ENSURE('\u00A0');
	JPT_ENSURE('�');

	return true;
}

export bool RunUnitTests_StringUtils()
{
	JPT_ENSURE(UnitTests_StringLen<jpt::String>());
	JPT_ENSURE(UnitTests_StringLen<jpt::WString>());

	JPT_ENSURE(UnitTests_ToCStr<jpt::String>());
	JPT_ENSURE(UnitTests_ToCStr<jpt::WString>());

	JPT_ENSURE(UnitTests_StrCpy<jpt::String>());
	JPT_ENSURE(UnitTests_StrCpy<jpt::WString>());

	JPT_ENSURE(RunUnitTests_StringUtils_IntegerToCStr());
	JPT_ENSURE(RunUnitTests_StringUtils_CStrToInteger());

	JPT_ENSURE(RunUnitTests_StringUtils_CStrToWStr());
	JPT_ENSURE(RunUnitTests_StringUtils_WStrToCStr());

	JPT_ENSURE(UnitTests_IsSpace());

	return true;
}