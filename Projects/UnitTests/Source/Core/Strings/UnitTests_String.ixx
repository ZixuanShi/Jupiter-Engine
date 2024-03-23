// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Minimal/Headers.h"
#include "Profiling/TimingProfiler.h"

#include <string>

export module UnitTests_String;

import jpt.DynamicArray;
import jpt.Math;
import jpt.RandomNumberGenerator;
import jpt.String;
import jpt.StringUtils;
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

bool UnitTest_String_Constructing()
{
	jpt::String str = "Hello";
	JPT_ENSURE(str == "Hello");

	str = "World";
	JPT_ENSURE(str == "World");

	str = "Hello World";
	JPT_ENSURE(str == "Hello World");

	return true;
}
bool UnitTest_WString_Constructing()
{
	jpt::WString str = L"Hello";
	JPT_ENSURE(str == L"Hello");

	str = L"World";
	JPT_ENSURE(str == L"World");

	str = L"Hello World";
	JPT_ENSURE(str == L"Hello World");

	return true;
}

bool UnitTest_String_Copying()
{
	jpt::String str = "Hello";
	jpt::String strCopy = str;
	JPT_ENSURE(str == strCopy);

	str = "World";
	JPT_ENSURE(str != strCopy);

	strCopy = str;
	JPT_ENSURE(str == strCopy);

	return true;
}
bool UnitTest_WString_Copying()
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

bool UnitTest_String_Concatenating()
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
bool UnitTest_WString_Concatenating()
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

bool UnitTest_String_Moving()
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
bool UnitTest_WString_Moving()
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

bool UnitTest_String_Comparison()
{
	jpt::String str = "Hello";
	JPT_ENSURE(str == "Hello");
	JPT_ENSURE(str != "World");
	JPT_ENSURE(str != "Hell");
	JPT_ENSURE(str != "Hell0");

	return true;
}
bool UnitTest_WString_Comparison()
{
	jpt::WString str = L"Hello";
	JPT_ENSURE(str == L"Hello");
	JPT_ENSURE(str != L"World");
	JPT_ENSURE(str != L"Hell");
	JPT_ENSURE(str != L"Hell0");

	return true;
}

bool UnitTest_String_Find()
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
bool UnitTest_WString_Find()
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

bool UnitTest_String_Replace()
{
	jpt::String str = "Hello World Jupiter Engine";
	str.Replace("Jupiter", "Mars");
	JPT_ENSURE(str == "Hello World Mars Engine");

	str.Replace("Hello", "Goodbye");
	JPT_ENSURE(str == "Goodbye World Mars Engine");

	str.Replace("Engine", "Universe");
	JPT_ENSURE(str == "Goodbye World Mars Universe");

	str.Replace("Goodbye World Mars Universe", "Hello World Jupiter Engine");
	JPT_ENSURE(str == "Hello World Jupiter Engine");

	return true;
}
bool UnitTest_WString_Replace()
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

	return true;
}

bool UnitTest_String_ConvertToNumber()
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
bool UnitTest_WString_ConvertToNumber()
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

bool UnitTest_String_Contains()
{
	jpt::String str = "Hello World Jupiter Engine";
	JPT_ENSURE(str.Contains("Hello"));
	JPT_ENSURE(str.Contains("World"));
	JPT_ENSURE(str.Contains("Jupiter"));
	JPT_ENSURE(str.Contains("Engine"));

	JPT_ENSURE(str.Contains("Jupiter Engine"));
	JPT_ENSURE(str.Contains("Hello World Jupiter Engine"));

	return true;
}
bool UnitTest_WString_Contains()
{
	jpt::WString str = L"Hello World Jupiter Engine";
	JPT_ENSURE(str.Contains(L"Hello"));
	JPT_ENSURE(str.Contains(L"World"));
	JPT_ENSURE(str.Contains(L"Jupiter"));
	JPT_ENSURE(str.Contains(L"Engine"));

	JPT_ENSURE(str.Contains(L"Jupiter Engine"));
	JPT_ENSURE(str.Contains(L"Hello World Jupiter Engine"));

	return true;
}

bool UnitTest_String_Split()
{
	jpt::String str = "Zero, One, Two, Three, Four, Five, Six";
	jpt::DynamicArray<jpt::String> substrs = str.Split(", ");
	jpt::DynamicArray<jpt::String> result = {"Zero", "One", "Two", "Three", "Four", "Five", "Six"};

	JPT_ENSURE(substrs == result);

	return true;
}
bool UnitTest_WString_Split()
{
	jpt::WString str = L"Zero, One, Two, Three, Four, Five, Six";
	jpt::DynamicArray<jpt::WString> substrs = str.Split(L", ");
	jpt::DynamicArray<jpt::WString> result = {L"Zero", L"One", L"Two", L"Three", L"Four", L"Five", L"Six"};

	JPT_ENSURE(substrs == result);

	return true;
}

bool UnitTest_String_Insert()
{
	jpt::String str = "Hello Jupiter Engine";

	str.Insert(" World", 5);
	JPT_ENSURE(str == "Hello World Jupiter Engine");
	JPT_ENSURE(str.Size() == 26);

	str.Insert(" World", 5);
	JPT_ENSURE(str == "Hello World World Jupiter Engine");
	JPT_ENSURE(str.Size() == 32);

	str.Insert(" World", 5);
	JPT_ENSURE(str == "Hello World World World Jupiter Engine");
	JPT_ENSURE(str.Size() == 38);

	str.Insert(" World", 5);
	JPT_ENSURE(str == "Hello World World World World Jupiter Engine");
	JPT_ENSURE(str.Size() == 44);

	return true;
}
bool UnitTest_WString_Insert()
{
	jpt::WString str = L"Hello Jupiter Engine";

	str.Insert(L" World", 5);
	JPT_ENSURE(str == L"Hello World Jupiter Engine");
	JPT_ENSURE(str.Size() == 26);

	str.Insert(L" World", 5);
	JPT_ENSURE(str == L"Hello World World Jupiter Engine");
	JPT_ENSURE(str.Size() == 32);

	str.Insert(L" World", 5);
	JPT_ENSURE(str == L"Hello World World World Jupiter Engine");
	JPT_ENSURE(str.Size() == 38);

	str.Insert(L" World", 5);
	JPT_ENSURE(str == L"Hello World World World World Jupiter Engine");
	JPT_ENSURE(str.Size() == 44);

	return true;
}

bool UnitTest_String_TrimLeft()
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
bool UnitTest_WString_TrimLeft()
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

bool UnitTest_String_TrimRight()
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
bool UnitTest_WString_TrimRight()
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

bool UnitTest_String_Iterator()
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

bool UnitTest_String_Format()
{
	jpt::String str = jpt::String::Format<32>("%s %d %s", "Hello", 42, "World");
	JPT_ENSURE(str == "Hello 42 World");

	str = jpt::String::Format<128>("%s, %s %d, %d, %d:%d:%d %s - %s", "Thursday", "December", 28, 2023, 10, 16, 56, "PM", "Jupiter Engine's Birthday");
	JPT_ENSURE(str == "Thursday, December 28, 2023, 10:16:56 PM - Jupiter Engine's Birthday");

	return true;
}

char g_smallBuffer[jpt::kSmallDataSize] = { 0 };
char* g_pBuffer = nullptr;

void Assign(const char* pSourceStr)
{
	const size_t length = jpt::GetCStrLength(pSourceStr);

	if (length < jpt::kSmallDataSize)
	{
		jpt::StrCpy(g_smallBuffer, jpt::kSmallDataSize, pSourceStr);
		g_pBuffer = g_smallBuffer;
	}
	else
	{
		if (jpt::GetCStrLength(g_pBuffer) >= jpt::kSmallDataSize)
		{
			delete[] g_pBuffer;
		}

		g_pBuffer = new char[length + 1];
		jpt::StrCpy(g_pBuffer, length + 1, pSourceStr);
	}
}

bool UnitTest_String_SSO()
{
	//char buffer[16] = { 0 };
	//jpt::StrCpy(buffer, 16, "Small Hello");
	//JPT_LOG(buffer);

	//std::memset(buffer, 0, 16);
	//JPT_LOG(buffer);

	//char* pBuffer = new char[] {"Hello"};
	//JPT_LOG(pBuffer);

	//if (pBuffer && pBuffer != buffer)
	//{
	//	delete[] pBuffer;
	//}
	//JPT_LOG(pBuffer);

	//pBuffer = buffer;
	//JPT_LOG(pBuffer == buffer);
	//JPT_LOG(pBuffer);

	Assign("");
	Assign("Hello");
	Assign("Hello World");
	Assign("Hello World Str");
	Assign("Hello World Stri");
	Assign("Hello World Jupiter Engine");
	Assign("Big String JSIOAJFOAISJFIOAJSFJISAF");

	if (jpt::GetCStrLength(g_pBuffer) >= jpt::kSmallDataSize)
	{
		delete[] g_pBuffer;
	}

	return true;
}

export bool RunUnitTests_String()
{
	JPT_ENSURE(UnitTest_String_Constructing());
	JPT_ENSURE(UnitTest_WString_Constructing());

	JPT_ENSURE(UnitTest_String_Copying());
	JPT_ENSURE(UnitTest_WString_Copying());

	JPT_ENSURE(UnitTest_String_Moving());
	JPT_ENSURE(UnitTest_WString_Moving());

	JPT_ENSURE(UnitTest_String_Concatenating());
	JPT_ENSURE(UnitTest_WString_Concatenating());

	JPT_ENSURE(UnitTest_String_Comparison());
	JPT_ENSURE(UnitTest_WString_Comparison());

	JPT_ENSURE(UnitTest_String_Find());
	JPT_ENSURE(UnitTest_WString_Find());

	JPT_ENSURE(UnitTest_String_Replace());
	JPT_ENSURE(UnitTest_WString_Replace());

	JPT_ENSURE(UnitTest_String_ConvertToNumber());
	JPT_ENSURE(UnitTest_WString_ConvertToNumber());

	JPT_ENSURE(UnitTest_String_Contains());
	JPT_ENSURE(UnitTest_WString_Contains());

	JPT_ENSURE(UnitTest_String_Split());
	JPT_ENSURE(UnitTest_WString_Split());

	JPT_ENSURE(UnitTest_String_Insert());
	JPT_ENSURE(UnitTest_WString_Insert());

	JPT_ENSURE(UnitTest_String_TrimLeft());
	JPT_ENSURE(UnitTest_WString_TrimLeft());

	JPT_ENSURE(UnitTest_String_TrimRight());
	JPT_ENSURE(UnitTest_WString_TrimRight());

	JPT_ENSURE(UnitTest_String_Iterator());
	JPT_ENSURE(UnitTest_String_Format());
	JPT_ENSURE(UnitTest_String_SSO());

	return true;
}