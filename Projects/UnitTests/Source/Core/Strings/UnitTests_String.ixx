// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Minimal/Headers.h"
#include "Profiling/TimingProfiler.h"

export module UnitTests_String;

import jpt.DynamicArray;
import jpt.Math;
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

template<class TString>
bool UnitTest_DefaultStringConstructing()
{
	using TChar = TString::TChar;

	TString defaultStr;
	JPT_ENSURE(defaultStr.IsEmpty());
	JPT_ENSURE(jpt::AreStringsSame(defaultStr.ConstBuffer(), JPT_GET_PROPER_STRING(TChar, )));
	JPT_ENSURE(jpt::AreStringsSame(defaultStr.Buffer(), JPT_GET_PROPER_STRING(TChar, )));
	
	defaultStr.Reserve(30);
	JPT_ENSURE(defaultStr.Capacity() == 30);
	JPT_ENSURE(defaultStr.IsEmpty());

	defaultStr.Append(JPT_GET_PROPER_STRING(TChar, Jupiter Engine), 14);
	defaultStr.Append(JPT_GET_PROPER_STRING(TChar, Zixuan), 6);
	JPT_ENSURE(defaultStr.Capacity() == 30);
	JPT_ENSURE(defaultStr == JPT_GET_PROPER_STRING(TChar, Jupiter EngineZixuan));

	defaultStr.Append(JPT_GET_PROPER_STRING(TChar, Testing Buffer Resizing), 24);
	JPT_ENSURE(defaultStr.Capacity() == 44);
	JPT_ENSURE(defaultStr.Size() == 44);
	JPT_ENSURE(defaultStr == JPT_GET_PROPER_STRING(TChar, Jupiter EngineZixuanTesting Buffer Resizing));

	defaultStr.Clear();
	JPT_ENSURE(defaultStr.IsEmpty());
	JPT_ENSURE(jpt::AreStringsSame(defaultStr.ConstBuffer(), JPT_GET_PROPER_STRING(TChar, ), 0));
	JPT_ENSURE(jpt::AreStringsSame(defaultStr.Buffer(), JPT_GET_PROPER_STRING(TChar, ), 0));

	defaultStr.Reserve(20);
	for (int32 i = 0; i < 10; ++i)
	{
		defaultStr.Append('C');
		defaultStr += 'A';
	}

	JPT_ENSURE(jpt::AreStringsSame(defaultStr.Buffer(), JPT_GET_PROPER_STRING(TChar, CACACACACACACACACACA), 0));

	return true;
}

template<class TString>
bool UnitTest_ConvertingString()
{
	using TChar = TString::TChar;

	TString numberStr(JPT_GET_PROPER_STRING(TChar, 114514));
	JPT_ENSURE(numberStr.ToInt() == 114514);

	numberStr = JPT_GET_PROPER_STRING(TChar, 114514.114514);
	JPT_ENSURE(jpt::AreValuesClose(numberStr.ToFloat(), 114514.114514f));

	return true;
}

template<class TString>
bool UnitTest_CopyingString()
{
	using TChar = TString::TChar;

	TString sourceStr(JPT_GET_PROPER_STRING(TChar, Jupiter Engine));
	JPT_ENSURE(sourceStr == JPT_GET_PROPER_STRING(TChar, Jupiter Engine));

	TString copiedStr(sourceStr);
	JPT_ENSURE(copiedStr == JPT_GET_PROPER_STRING(TChar, Jupiter Engine));
	JPT_ENSURE(sourceStr == copiedStr);

	sourceStr = JPT_GET_PROPER_STRING(TChar, Reassigned String);
	JPT_ENSURE(sourceStr == JPT_GET_PROPER_STRING(TChar, Reassigned String));
	JPT_ENSURE(copiedStr == JPT_GET_PROPER_STRING(TChar, Jupiter Engine));

	copiedStr = sourceStr;
	JPT_ENSURE(sourceStr == JPT_GET_PROPER_STRING(TChar, Reassigned String));
	JPT_ENSURE(copiedStr == JPT_GET_PROPER_STRING(TChar, Reassigned String));

	copiedStr += JPT_GET_PROPER_STRING(TChar, Further Testing);
	JPT_ENSURE(copiedStr == JPT_GET_PROPER_STRING(TChar, Reassigned StringFurther Testing));

	sourceStr.CopyString(copiedStr);
	JPT_ENSURE(sourceStr == JPT_GET_PROPER_STRING(TChar, Reassigned StringFurther Testing));

	return true;
}

template<class TString>
bool UnitTest_MovingString()
{
	using TChar = TString::TChar;

	TString sourceStr(JPT_GET_PROPER_STRING(TChar, Jupiter Engine));
	JPT_ENSURE(sourceStr == JPT_GET_PROPER_STRING(TChar, Jupiter Engine));

	TString movedStr(jpt::Move(sourceStr));
	JPT_ENSURE(movedStr == JPT_GET_PROPER_STRING(TChar, Jupiter Engine));
	JPT_ENSURE(sourceStr.IsEmpty());
	JPT_ENSURE(sourceStr.ConstBuffer() == nullptr);

	sourceStr = JPT_GET_PROPER_STRING(TChar, Reassigned String); 
	JPT_ENSURE(sourceStr == JPT_GET_PROPER_STRING(TChar, Reassigned String));
	JPT_ENSURE(movedStr == JPT_GET_PROPER_STRING(TChar, Jupiter Engine));

	movedStr = jpt::Move(sourceStr);
	JPT_ENSURE(sourceStr.IsEmpty());
	JPT_ENSURE(sourceStr.ConstBuffer() == nullptr);
	JPT_ENSURE(movedStr == JPT_GET_PROPER_STRING(TChar, Reassigned String));

	movedStr += JPT_GET_PROPER_STRING(TChar, Further Testing);
	JPT_ENSURE(movedStr == JPT_GET_PROPER_STRING(TChar, Reassigned StringFurther Testing));

	sourceStr.MoveString(jpt::Move(movedStr));
	JPT_ENSURE(sourceStr == JPT_GET_PROPER_STRING(TChar, Reassigned StringFurther Testing));
	JPT_ENSURE(movedStr.IsEmpty());
	JPT_ENSURE(movedStr.ConstBuffer() == nullptr);

	return true;
}

template<class TString>
bool UnitTest_SubStr()
{
	using TChar = TString::TChar;

	TString sourceStr(JPT_GET_PROPER_STRING(TChar, Jupiter Engine));
	TString subStr;

	subStr = sourceStr.SubStr(0, 7);
	JPT_ENSURE(subStr == JPT_GET_PROPER_STRING(TChar, Jupiter));

	subStr = sourceStr.SubStr(8, 6);
	JPT_ENSURE(subStr == JPT_GET_PROPER_STRING(TChar, Engine));

	return true;
}

template<class TString>
bool UnitTest_ToString()
{
	using TChar = TString::TChar;

	TString string = jpt::ToString<TString>(114151);
	JPT_ENSURE(string == JPT_GET_PROPER_STRING(TChar, 114151));

	string = jpt::ToString<TString>(114151.114);
	JPT_ENSURE(string == JPT_GET_PROPER_STRING(TChar, 114151.114));

	// TODO [zshi]: Add containers, vector, and matrix when finished

	return true;
}

bool UnitTest_ToString2()
{
	struct Foo
	{
		int32 m_data = 0;

		Foo() = default;
		Foo(int32 data) :m_data(data) {}

		jpt::String ToString() const
		{
			return jpt::ToString(m_data) + " Foo";
		}
	};

	Foo foo;

	JPT_ENSURE(jpt::ToString(foo) == "0 Foo");
	JPT_ENSURE(jpt::ToString(0) == "0");
	JPT_ENSURE(jpt::ToString(Foo{32}) == "32 Foo");

	return true;
}

template<class TString>
bool UnitTest_StringFinding()
{
	using TChar = TString::TChar;
	TString string(JPT_GET_PROPER_STRING(TChar, This is a test string for Jupiter Engine's jpt::BasicString'));

	size_t isPosition = string.Find(JPT_GET_PROPER_STRING(TChar, is));
	JPT_ENSURE(isPosition == 2);

	size_t ePosition = string.Find(JPT_GET_PROPER_STRING(TChar, e), 20);
	JPT_ENSURE(ePosition == 31);

	size_t lastIsPosition = string.FindLastOf(JPT_GET_PROPER_STRING(TChar, is));
	JPT_ENSURE(lastIsPosition == 5);

	size_t lastEPosition = string.FindLastOf(JPT_GET_PROPER_STRING(TChar, e));
	JPT_ENSURE(lastEPosition == 39);

	size_t last20EPosition = string.FindLastOf(JPT_GET_PROPER_STRING(TChar, e), 20, 33);
	JPT_ENSURE(last20EPosition == 31);

	size_t noExistString = string.Find(JPT_GET_PROPER_STRING(TChar, Doesnt Exist String));
	JPT_ENSURE(noExistString == jpt::npos);

	return true;
}

template<class TString>
bool UnitTest_StringReplace()
{
	using TChar = TString::TChar;
	TString string(JPT_GET_PROPER_STRING(TChar, This is a test string for Jupiter Engine jpt::BasicString));

	string.Replace(JPT_GET_PROPER_STRING(TChar, e), JPT_GET_PROPER_STRING(TChar, LOL));
	JPT_ENSURE(string == JPT_GET_PROPER_STRING(TChar, This is a tLOLst string for JupitLOLr EnginLOL jpt::BasicString));
						
	string = JPT_GET_PROPER_STRING(TChar, This is a test string for Jupiter Engine jpt::BasicString);
	string.Replace(JPT_GET_PROPER_STRING(TChar, e), JPT_GET_PROPER_STRING(TChar, LOL), 20, 42);
	JPT_ENSURE(string == JPT_GET_PROPER_STRING(TChar, This is a test string for JupitLOLr EnginLOL jpt::BasicString));

	string = JPT_GET_PROPER_STRING(TChar, This is a test string for Jupiter Engine jpt::BasicString);
	string.Replace(JPT_GET_PROPER_STRING(TChar, e), JPT_GET_PROPER_STRING(TChar, LOL), 20, 32);
	JPT_ENSURE(string == JPT_GET_PROPER_STRING(TChar, This is a test string for JupitLOLr Engine jpt::BasicString));

	TString replaced = string.GetReplaced(JPT_GET_PROPER_STRING(TChar, e), JPT_GET_PROPER_STRING(TChar, LOL));
	JPT_ENSURE(replaced == JPT_GET_PROPER_STRING(TChar, This is a tLOLst string for JupitLOLr EnginLOL jpt::BasicString));
	
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

bool UnitTest_String_Split()
{
	jpt::String str = "Zero, One, Two, Three, Four, Five, Six";
	jpt::DynamicArray<jpt::String> substrs = str.Split(", ");
	jpt::DynamicArray<jpt::String> result = {"Zero", "One", "Two", "Three", "Four", "Five", "Six"};

	JPT_ENSURE(substrs == result);

	return true;
}

bool UnitTest_String_SSO()
{
	//jpt::String str = "Small Str";

	//char buffer[16];
	//std::memset(buffer, 0, 16);

	//char* pBuffer = nullptr;

	//char* pRightBuffer = buffer;

	return true;
}

export bool RunUnitTests_String()
{
	JPT_SCOPED_TIMING_PROFILER(UnitTests_String);

	JPT_ENSURE(UnitTest_DefaultStringConstructing<jpt::String>());
	JPT_ENSURE(UnitTest_DefaultStringConstructing<jpt::WString>());

	JPT_ENSURE(UnitTest_ConvertingString<jpt::String>());
	JPT_ENSURE(UnitTest_ConvertingString<jpt::WString>());

	JPT_ENSURE(UnitTest_CopyingString<jpt::String>());
	JPT_ENSURE(UnitTest_CopyingString<jpt::WString>());

	JPT_ENSURE(UnitTest_MovingString<jpt::String>());
	JPT_ENSURE(UnitTest_MovingString<jpt::WString>());

	JPT_ENSURE(UnitTest_SubStr<jpt::String>());
	JPT_ENSURE(UnitTest_SubStr<jpt::WString>());

	JPT_ENSURE(UnitTest_ToString<jpt::String>());
	JPT_ENSURE(UnitTest_ToString<jpt::WString>());
	JPT_ENSURE(UnitTest_ToString2());

	JPT_ENSURE(UnitTest_StringFinding<jpt::String>());
	JPT_ENSURE(UnitTest_StringFinding<jpt::WString>());

	JPT_ENSURE(UnitTest_StringReplace<jpt::String>());
	JPT_ENSURE(UnitTest_StringReplace<jpt::WString>());

	JPT_ENSURE(UnitTest_String_Iterator());
	JPT_ENSURE(UnitTest_String_Format());
	JPT_ENSURE(UnitTest_String_Split());

	JPT_ENSURE(UnitTest_String_SSO());

	return true;
}