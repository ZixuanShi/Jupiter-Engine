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

bool UnitTest_String_Split()
{
	jpt::String str = "Zero, One, Two, Three, Four, Five, Six";
	jpt::DynamicArray<jpt::String> substrs = str.Split(", ");
	jpt::DynamicArray<jpt::String> result = {"Zero", "One", "Two", "Three", "Four", "Five", "Six"};

	JPT_ENSURE(substrs == result);

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
	JPT_ENSURE(UnitTest_String_Iterator());
	JPT_ENSURE(UnitTest_String_Format());
	JPT_ENSURE(UnitTest_String_Split());

	JPT_ENSURE(UnitTest_String_SSO());

	return true;
}