// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Minimal/CoreHeaders.h"
#include "Profiling/TimingProfiler.h"

#include <string>

export module UnitTests_String;

import jpt.DynamicArray;
import jpt.Math;
import jpt.Rand;
import jpt.String;
import jpt.String.Helpers;
import jpt.StringView;
import jpt.ToString;
import jpt.TypeDefs;
import jpt.Utilities;

auto locHelper = [](size_t i) -> const char*
	{
		switch (i)
		{
		case 0: return "Zero";
		case 1: return "One";
		case 2: return "Two";
		case 3: return "Three";
		case 4: return "Four";
		case 5: return "Five";
		case 6: return "Six";
		case 7: return "Seven";
		case 8: return "Eight";
		case 9: return "Nine";
		case 10: return "Ten";
		case 11: return "Eleven";
		case 12: return "Twelve";

		default: return "Error";
		}
	};

bool UnitTests_String_Constructing()
{
	jpt::String str = "Hello";
	JPT_ENSURE(str == "Hello");

	str = "World";
	JPT_ENSURE(str == "World");

	str = "Hello World";
	JPT_ENSURE(str == "Hello World");

	return true;
}
bool UnitTests_WString_Constructing()
{
	jpt::WString str = L"Hello";
	JPT_ENSURE(str == L"Hello");

	str = L"World";
	JPT_ENSURE(str == L"World");

	str = L"Hello World";
	JPT_ENSURE(str == L"Hello World");

	return true;
}

bool UnitTests_String_Copying()
{
	jpt::String str = "Hello";
	jpt::String strCopy = str;
	JPT_ENSURE(str == strCopy);

	str = "World";
	JPT_ENSURE(str != strCopy);

	strCopy = str;
	JPT_ENSURE(str == strCopy);

	str.CopyString("12345678901234567890", 20);
	str.CopyString("12345678901234567891", 20);

	return true;
}
bool UnitTests_WString_Copying()
{
	jpt::WString str = L"Hello";
	jpt::WString strCopy = str;
	JPT_ENSURE(str == strCopy);

	str = L"World";
	JPT_ENSURE(str != strCopy);

	strCopy = str;
	JPT_ENSURE(str == strCopy);

	return true;
}

bool UnitTests_String_Concatenating()
{
	jpt::String str = "Hello";
	str += " World";
	JPT_ENSURE(str == "Hello World");

	str += " Jupiter Engine";
	JPT_ENSURE(str == "Hello World Jupiter Engine");

	str.Append(" zshi");
	JPT_ENSURE(str == "Hello World Jupiter Engine zshi");

	return true;
}
bool UnitTests_WString_Concatenating()
{
	jpt::WString str = L"Hello";
	str += L" World";
	JPT_ENSURE(str == L"Hello World");

	str += L" Jupiter Engine";
	JPT_ENSURE(str == L"Hello World Jupiter Engine");

	str.Append(L" zshi");
	JPT_ENSURE(str == L"Hello World Jupiter Engine zshi");

	return true;
}

bool UnitTests_String_Moving()
{
	jpt::String str = "Hello";
	jpt::String strMove = jpt::Move(str);
	JPT_ENSURE(strMove == "Hello");
	JPT_ENSURE(str.IsEmpty());
	JPT_ENSURE(str == "");

	str = jpt::Move(strMove);
	JPT_ENSURE(str == "Hello");
	JPT_ENSURE(strMove.IsEmpty());

	return true;
}
bool UnitTests_WString_Moving()
{
	jpt::WString str = L"Hello";
	jpt::WString strMove = jpt::Move(str);
	JPT_ENSURE(strMove == L"Hello");
	JPT_ENSURE(str.IsEmpty());
	JPT_ENSURE(str == L"");

	str = jpt::Move(strMove);
	JPT_ENSURE(str == L"Hello");
	JPT_ENSURE(strMove.IsEmpty());

	return true;
}

bool UnitTests_String_Comparison()
{
	jpt::String str = "Hello";
	JPT_ENSURE(str == "Hello");
	JPT_ENSURE(str != "World");
	JPT_ENSURE(str != "Hell");
	JPT_ENSURE(str != "Hell0");

	return true;
}
bool UnitTests_WString_Comparison()
{
	jpt::WString str = L"Hello";
	JPT_ENSURE(str == L"Hello");
	JPT_ENSURE(str != L"World");
	JPT_ENSURE(str != L"Hell");
	JPT_ENSURE(str != L"Hell0");

	return true;
}

bool UnitTests_String_Find()
{
	jpt::String str = "Hello World Jupiter Engine";
	JPT_ENSURE(str.Find("Hello") == 0);
	JPT_ENSURE(str.Find("World") == 6);
	JPT_ENSURE(str.Find("Jupiter") == 12);
	JPT_ENSURE(str.Find("Engine") == 20);

	JPT_ENSURE(str.Find("Jupiter Engine") == 12);
	JPT_ENSURE(str.Find("Hello World Jupiter Engine") == 0);

	return true;
}
bool UnitTests_WString_Find()
{
	jpt::WString str = L"Hello World Jupiter Engine";
	JPT_ENSURE(str.Find(L"Hello") == 0);
	JPT_ENSURE(str.Find(L"World") == 6);
	JPT_ENSURE(str.Find(L"Jupiter") == 12);
	JPT_ENSURE(str.Find(L"Engine") == 20);

	JPT_ENSURE(str.Find(L"Jupiter Engine") == 12);
	JPT_ENSURE(str.Find(L"Hello World Jupiter Engine") == 0);

	return true;
}

bool UnitTests_String_Replace()
{
	// Single word
	jpt::String str = "Hello World Jupiter Engine";
	str.Replace("Jupiter", "Mars");
	JPT_ENSURE(str == "Hello World Mars Engine");

	str.Replace("Hello", "Goodbye");
	JPT_ENSURE(str == "Goodbye World Mars Engine");

	str.Replace("Engine", "Universe");
	JPT_ENSURE(str == "Goodbye World Mars Universe");

	str.Replace("Goodbye World Mars Universe", "Hello World Jupiter Engine");
	JPT_ENSURE(str == "Hello World Jupiter Engine");

	str.Replace("e", "En");
	JPT_ENSURE(str == "HEnllo World JupitEnr EnginEn");

	str.Replace("En", "End", 10, 24);
	JPT_ENSURE(str == "HEnllo World JupitEndr EndginEn");

	// Multiple words
	str = "Hello World Jupiter Engine";
	str.Replace({ "Hello", "Jupiter" }, "YOLO");
	JPT_ENSURE(str == "YOLO World YOLO Engine");

	str.Replace({ "YOLO", "Engine" }, "Hello Jupiter");
	JPT_ENSURE(str == "Hello Jupiter World Hello Jupiter Hello Jupiter");
	
	return true;
}
bool UnitTests_WString_Replace()
{
	jpt::WString str = L"Hello World Jupiter Engine";
	str.Replace(L"Jupiter", L"Mars");
	JPT_ENSURE(str == L"Hello World Mars Engine");

	str.Replace(L"Hello", L"Goodbye");
	JPT_ENSURE(str == L"Goodbye World Mars Engine");

	str.Replace(L"Engine", L"Universe");
	JPT_ENSURE(str == L"Goodbye World Mars Universe");

	str.Replace(L"Goodbye World Mars Universe", L"Hello World Jupiter Engine");
	JPT_ENSURE(str == L"Hello World Jupiter Engine");

	str.Replace(L"e", L"En");
	JPT_ENSURE(str == L"HEnllo World JupitEnr EnginEn");

	str.Replace(L"En", L"End", 10, 24);
	JPT_ENSURE(str == L"HEnllo World JupitEndr EndginEn");

	// Multiple words
	str = L"Hello World Jupiter Engine";
 	str.Replace({ L"Hello", L"Jupiter" }, L"YOLO");
	JPT_ENSURE(str == L"YOLO World YOLO Engine");

	str.Replace({ L"YOLO", L"Engine" }, L"Hello Jupiter");
	JPT_ENSURE(str == L"Hello Jupiter World Hello Jupiter Hello Jupiter");

	return true;
}

bool UnitTests_String_ConvertToNumber()
{
	jpt::String str = "42";
	int32 number = str.ToInt();
	JPT_ENSURE(number == 42);

	str = "3.14159";
	float32 fNumber = str.ToFloat();
	JPT_ENSURE(jpt::AreValuesClose(fNumber, 3.14159f));

	str = "3.14159";
	float64 dNumber = str.ToFloat();
	JPT_ENSURE(jpt::AreValuesClose(dNumber, 3.14159));

	return true;
}
bool UnitTests_WString_ConvertToNumber()
{
	jpt::WString str = L"42";
	int32 number = str.ToInt();
	JPT_ENSURE(number == 42);

	str = L"3.14159";
	float32 fNumber = str.ToFloat();
	JPT_ENSURE(jpt::AreValuesClose(fNumber, 3.14159f));

	str = L"3.14159";
	float64 dNumber = str.ToFloat();
	JPT_ENSURE(jpt::AreValuesClose(dNumber, 3.14159));

	return true;
}

bool UnitTests_String_Has()
{
	jpt::String str = "Hello World Jupiter Engine";
	JPT_ENSURE(str.Has("Hello"));
	JPT_ENSURE(str.Has("World"));
	JPT_ENSURE(str.Has("Jupiter"));
	JPT_ENSURE(str.Has("Engine"));

	JPT_ENSURE(str.Has("Jupiter Engine"));
	JPT_ENSURE(str.Has("Hello World Jupiter Engine"));

	return true;
}
bool UnitTests_WString_Has()
{
	jpt::WString str = L"Hello World Jupiter Engine";
	JPT_ENSURE(str.Has(L"Hello"));
	JPT_ENSURE(str.Has(L"World"));
	JPT_ENSURE(str.Has(L"Jupiter"));
	JPT_ENSURE(str.Has(L"Engine"));

	JPT_ENSURE(str.Has(L"Jupiter Engine"));
	JPT_ENSURE(str.Has(L"Hello World Jupiter Engine"));

	return true;
}

bool UnitTests_String_Split()
{
	jpt::String str = "Zero, One, Two, Three, Four, Five, Six";
	jpt::DynamicArray<jpt::String> substrs = str.Split(", ");
	jpt::DynamicArray<jpt::String> result = {"Zero", "One", "Two", "Three", "Four", "Five", "Six"};

	JPT_ENSURE(substrs == result);

	return true;
}
bool UnitTests_WString_Split()
{
	jpt::WString str = L"Zero, One, Two, Three, Four, Five, Six";
	jpt::DynamicArray<jpt::WString> substrs = str.Split(L", ");
	jpt::DynamicArray<jpt::WString> result = {L"Zero", L"One", L"Two", L"Three", L"Four", L"Five", L"Six"};

	JPT_ENSURE(substrs == result);

	return true;
}

bool UnitTests_String_Insert()
{
	jpt::String str = "Hello Jupiter Engine";

	str.Insert(" World", 5);
	JPT_ENSURE(str == "Hello World Jupiter Engine");
	JPT_ENSURE(str.Count() == 26);

	str.Insert(" World", 5);
	JPT_ENSURE(str == "Hello World World Jupiter Engine");
	JPT_ENSURE(str.Count() == 32);

	str.Insert(" World", 5);
	JPT_ENSURE(str == "Hello World World World Jupiter Engine");
	JPT_ENSURE(str.Count() == 38);

	str.Insert(" World", 5);
	JPT_ENSURE(str == "Hello World World World World Jupiter Engine");
	JPT_ENSURE(str.Count() == 44);

	return true;
}
bool UnitTests_WString_Insert()
{
	jpt::WString str = L"Hello Jupiter Engine";

	str.Insert(L" World", 5);
	JPT_ENSURE(str == L"Hello World Jupiter Engine");
	JPT_ENSURE(str.Count() == 26);

	str.Insert(L" World", 5);
	JPT_ENSURE(str == L"Hello World World Jupiter Engine");
	JPT_ENSURE(str.Count() == 32);

	str.Insert(L" World", 5);
	JPT_ENSURE(str == L"Hello World World World Jupiter Engine");
	JPT_ENSURE(str.Count() == 38);

	str.Insert(L" World", 5);
	JPT_ENSURE(str == L"Hello World World World World Jupiter Engine");
	JPT_ENSURE(str.Count() == 44);

	return true;
}

bool UnitTests_String_TrimLeft()
{
	jpt::String str = "   Hello Jupiter Engine";
	str.TrimLeft();
	JPT_ENSURE(str == "Hello Jupiter Engine");

	str = "   Hello Jupiter Engine";
	str.TrimLeft(3);
	JPT_ENSURE(str == "Hello Jupiter Engine");

	str = "   Hello Jupiter Engine";
	str.TrimLeft(9);
	JPT_ENSURE(str == "Jupiter Engine");

	return true;
}
bool UnitTests_WString_TrimLeft()
{
	jpt::WString str = L"   Hello Jupiter Engine";
	str.TrimLeft();
	JPT_ENSURE(str == L"Hello Jupiter Engine");

	str = L"   Hello Jupiter Engine";
	str.TrimLeft(3);
	JPT_ENSURE(str == L"Hello Jupiter Engine");

	str = L"   Hello Jupiter Engine";
	str.TrimLeft(9);
	JPT_ENSURE(str == L"Jupiter Engine");

	return true;
}

bool UnitTests_String_TrimRight()
{
	jpt::String str = "Hello Jupiter Engine   ";
	str.TrimRight();
	JPT_ENSURE(str == "Hello Jupiter Engine");

	str = "Hello Jupiter Engine";
	str.TrimRight(5);
	JPT_ENSURE(str == "Hello");

	str = "Hello Jupiter Engine";
	str.TrimRight(13);
	JPT_ENSURE(str == "Hello Jupiter");

	return true;
}
bool UnitTests_WString_TrimRight()
{
	jpt::WString str = L"Hello Jupiter Engine   ";
	str.TrimRight();
	JPT_ENSURE(str == L"Hello Jupiter Engine");

	str = L"Hello Jupiter Engine";
	str.TrimRight(5);
	JPT_ENSURE(str == L"Hello");

	str = L"Hello Jupiter Engine";
	str.TrimRight(13);
	JPT_ENSURE(str == L"Hello Jupiter");

	return true;
}

bool UnitTests_String_Iterator()
{
	jpt::String str("0123456789");

	int32 i = 0;
	for (char c : str)
	{
		//JPT_LOG(c);

		jpt::String s = jpt::ToString(i);
		JPT_ENSURE(s[0] == c);

		++i;
	}

	str = "Jupiter Jupiter Jupiter Jupiter ";
	for (auto itr = str.begin(); itr < str.end(); itr += 8)
	{
		jpt::StringView strView(&*itr, 8);
		JPT_ENSURE(strView == "Jupiter ");
	}

	return true;
}
bool UnitTests_WString_Iterator()
{
	jpt::WString str(L"0123456789");

	int32 i = 0;
	for (wchar_t c : str)
	{
		//JPT_LOG(c);

		jpt::WString s = jpt::ToString<jpt::WString>(i);
		JPT_ENSURE(s[0] == c);

		++i;
	}

	str = L"Jupiter Jupiter Jupiter Jupiter ";
	for (auto itr = str.begin(); itr < str.end(); itr += 8)
	{
		jpt::WString strView(&*itr, 8);
		JPT_ENSURE(strView == L"Jupiter ");
	}

	return true;
}

bool UnitTests_String_Format()
{
	jpt::String str = jpt::String::Format<32>("%s %d %s", "Hello", 42, "World");
	JPT_ENSURE(str == "Hello 42 World");

	str = jpt::String::Format<128>("%s, %s %d, %d, %d:%d:%d %s - %s", "Thursday", "December", 28, 2023, 10, 16, 56, "PM", "Jupiter Engine's Birthday");
	JPT_ENSURE(str == "Thursday, December 28, 2023, 10:16:56 PM - Jupiter Engine's Birthday");

	return true;
}
bool UnitTests_WString_Format()
{
	jpt::WString str = jpt::WString::Format<32>(L"%s %d %s", L"Hello", 42, L"World");
	JPT_ENSURE(str == L"Hello 42 World");

	str = jpt::WString::Format<128>(L"%s, %s %d, %d, %d:%d:%d %s - %s", L"Thursday", L"December", 28, 2023, 10, 16, 56, L"PM", L"Jupiter Engine's Birthday");
	JPT_ENSURE(str == L"Thursday, December 28, 2023, 10:16:56 PM - Jupiter Engine's Birthday");

	return true;
}

bool UnitTests_String_MakeUpper()
{
	jpt::String str = "Hello World Jupiter Engine";
	str.MakeUpper();

	JPT_ENSURE(str == "HELLO WORLD JUPITER ENGINE");

	JPT_ENSURE(jpt::String::GetUpper("Hello World Jupiter Engine") == "HELLO WORLD JUPITER ENGINE");

	return true;
}
bool UnitTests_WString_MakeUpper()
{
	jpt::WString str = L"Hello World Jupiter Engine";
	str.MakeUpper();

	JPT_ENSURE(str == L"HELLO WORLD JUPITER ENGINE");

	JPT_ENSURE(jpt::WString::GetUpper(L"Hello World Jupiter Engine") == L"HELLO WORLD JUPITER ENGINE");

	return true;
}

bool UnitTests_String_MakeLower()
{
	jpt::String str = "Hello World Jupiter Engine";
	str.MakeLower();

	JPT_ENSURE(str == "hello world jupiter engine");

	JPT_ENSURE(jpt::String::GetLower("Hello World Jupiter Engine") == "hello world jupiter engine");

	return true;
}
bool UnitTests_WString_MakeLower()
{
	jpt::WString str = L"Hello World Jupiter Engine";
	str.MakeLower();

	JPT_ENSURE(str == L"hello world jupiter engine");

	JPT_ENSURE(jpt::WString::GetLower(L"Hello World Jupiter Engine") == L"hello world jupiter engine");

	return true;
}

bool UnitTests_String_ToString()
{
	JPT_ENSURE(jpt::ToString(L"") == "");
	JPT_ENSURE(jpt::ToString(L"Hello") == "Hello");
	JPT_ENSURE(jpt::ToString(L"World") == "World");
	JPT_ENSURE(jpt::ToString(L"Hello World") == "Hello World");

	return true;
}
bool UnitTests_String_ToWString()
{
	JPT_ENSURE(jpt::ToWString("") == L"");
	JPT_ENSURE(jpt::ToWString("Hello") == L"Hello");
	JPT_ENSURE(jpt::ToWString("World") == L"World");
	JPT_ENSURE(jpt::ToWString("Hello World") == L"Hello World");

	return true;
}

bool UnitTests_String_StringCompareWCStr()
{
	jpt::String str = "Hello";
	JPT_ENSURE(str == L"Hello");
	JPT_ENSURE(str != L"World");

	return true;
}
bool UnitTests_String_WStringCompareCStr()
{
	jpt::WString str = L"Hello";
	JPT_ENSURE(str == "Hello");
	JPT_ENSURE(str != "World");

	return true;
}

export bool RunUnitTests_String()
{
	//JPT_SCOPED_TIMING_PROFILER("String");

	//for (size_t i = 0; i < 10'000; ++i)
	{
		JPT_ENSURE(UnitTests_String_Constructing());
		JPT_ENSURE(UnitTests_WString_Constructing());

		JPT_ENSURE(UnitTests_String_Copying());
		JPT_ENSURE(UnitTests_WString_Copying());

		JPT_ENSURE(UnitTests_String_Moving());
		JPT_ENSURE(UnitTests_WString_Moving());

		JPT_ENSURE(UnitTests_String_Concatenating());
		JPT_ENSURE(UnitTests_WString_Concatenating());

		JPT_ENSURE(UnitTests_String_Comparison());
		JPT_ENSURE(UnitTests_WString_Comparison());

		JPT_ENSURE(UnitTests_String_Find());
		JPT_ENSURE(UnitTests_WString_Find());

		JPT_ENSURE(UnitTests_String_Replace());
		JPT_ENSURE(UnitTests_WString_Replace());

		JPT_ENSURE(UnitTests_String_ConvertToNumber());
		JPT_ENSURE(UnitTests_WString_ConvertToNumber());

		JPT_ENSURE(UnitTests_String_Has());
		JPT_ENSURE(UnitTests_WString_Has());

		JPT_ENSURE(UnitTests_String_Split());
		JPT_ENSURE(UnitTests_WString_Split());

		JPT_ENSURE(UnitTests_String_Insert());
		JPT_ENSURE(UnitTests_WString_Insert());

		JPT_ENSURE(UnitTests_String_TrimLeft());
		JPT_ENSURE(UnitTests_WString_TrimLeft());

		JPT_ENSURE(UnitTests_String_TrimRight());
		JPT_ENSURE(UnitTests_WString_TrimRight());

		JPT_ENSURE(UnitTests_String_Iterator());
		JPT_ENSURE(UnitTests_WString_Iterator());

		JPT_ENSURE(UnitTests_String_Format());
		JPT_ENSURE(UnitTests_WString_Format());

		JPT_ENSURE(UnitTests_String_MakeUpper());
		JPT_ENSURE(UnitTests_WString_MakeUpper());

		JPT_ENSURE(UnitTests_String_MakeLower());
		JPT_ENSURE(UnitTests_WString_MakeLower());

		JPT_ENSURE(UnitTests_String_ToString());
		JPT_ENSURE(UnitTests_String_ToWString());

		JPT_ENSURE(UnitTests_String_StringCompareWCStr());
		JPT_ENSURE(UnitTests_String_WStringCompareCStr());
	}

	return true;
}