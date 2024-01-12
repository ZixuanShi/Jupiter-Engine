// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Building/Headers.h"

export module StringUnitTests;

import jpt.CoreModules;

template<class StringT>
bool UnitTest_DefaultStringConstructing()
{
	using CharT = StringT::CharT;

	StringT defaultStr;
	JPT_RETURN_FALSE_IF_ERROR(!defaultStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(defaultStr.ConstBuffer(), JPT_GET_PROPER_STRING(CharT, ), 0), "");
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(defaultStr.Buffer(), JPT_GET_PROPER_STRING(CharT, ), 0), "");
	
	defaultStr.Reserve(30);
	JPT_RETURN_FALSE_IF_ERROR(defaultStr.Capacity() != 30, "");
	JPT_RETURN_FALSE_IF_ERROR(!defaultStr.IsEmpty(), "");

	defaultStr.Append(JPT_GET_PROPER_STRING(CharT, Jupiter Engine), 14);
	defaultStr.Append(JPT_GET_PROPER_STRING(CharT, Zixuan), 6);
	JPT_RETURN_FALSE_IF_ERROR(defaultStr.Capacity() != 30, "");
	JPT_RETURN_FALSE_IF_ERROR(defaultStr != JPT_GET_PROPER_STRING(CharT, Jupiter EngineZixuan), "");

	defaultStr.Append(JPT_GET_PROPER_STRING(CharT, Testing Buffer Resizing), 24);
	JPT_RETURN_FALSE_IF_ERROR(defaultStr.Capacity() != 44, "");
	JPT_RETURN_FALSE_IF_ERROR(defaultStr.Size() != 44, "");
	JPT_RETURN_FALSE_IF_ERROR(defaultStr != JPT_GET_PROPER_STRING(CharT, Jupiter EngineZixuanTesting Buffer Resizing), "");

	defaultStr.Clear();
	JPT_RETURN_FALSE_IF_ERROR(!defaultStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(defaultStr.ConstBuffer(), JPT_GET_PROPER_STRING(CharT, ), 0), "");
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreStringsSame(defaultStr.Buffer(), JPT_GET_PROPER_STRING(CharT, ), 0), "");

	return true;
}

template<class StringT>
bool UnitTest_ConvertingString()
{
	using CharT = StringT::CharT;

	StringT numberStr(JPT_GET_PROPER_STRING(CharT, 114514));
	JPT_RETURN_FALSE_IF_ERROR(numberStr.ToInt() != 114514, "");

	numberStr = JPT_GET_PROPER_STRING(CharT, 114514.114514);
	JPT_RETURN_FALSE_IF_ERROR(!jpt::AreValuesClose(numberStr.ToFloat(), 114514.114514f), "");

	return true;
}

template<class StringT>
bool UnitTest_CopyingString()
{
	using CharT = StringT::CharT;

	StringT sourceStr(JPT_GET_PROPER_STRING(CharT, Jupiter Engine));
	JPT_RETURN_FALSE_IF_ERROR(sourceStr != JPT_GET_PROPER_STRING(CharT, Jupiter Engine), "");

	StringT copiedStr(sourceStr);
	JPT_RETURN_FALSE_IF_ERROR(copiedStr != JPT_GET_PROPER_STRING(CharT, Jupiter Engine), "");
	JPT_RETURN_FALSE_IF_ERROR(sourceStr != copiedStr, "");

	sourceStr = JPT_GET_PROPER_STRING(CharT, Reassigned String);
	JPT_RETURN_FALSE_IF_ERROR(sourceStr != JPT_GET_PROPER_STRING(CharT, Reassigned String), "");
	JPT_RETURN_FALSE_IF_ERROR(copiedStr != JPT_GET_PROPER_STRING(CharT, Jupiter Engine), "");

	copiedStr = sourceStr;
	JPT_RETURN_FALSE_IF_ERROR(sourceStr != JPT_GET_PROPER_STRING(CharT, Reassigned String), "");
	JPT_RETURN_FALSE_IF_ERROR(copiedStr != JPT_GET_PROPER_STRING(CharT, Reassigned String), "");

	copiedStr += JPT_GET_PROPER_STRING(CharT, Further Testing);
	JPT_RETURN_FALSE_IF_ERROR(copiedStr != JPT_GET_PROPER_STRING(CharT, Reassigned StringFurther Testing), "");

	sourceStr.CopyString(copiedStr);
	JPT_RETURN_FALSE_IF_ERROR(sourceStr != JPT_GET_PROPER_STRING(CharT, Reassigned StringFurther Testing), "");

	return true;
}

template<class StringT>
bool UnitTest_MovingString()
{
	using CharT = StringT::CharT;

	StringT sourceStr(JPT_GET_PROPER_STRING(CharT, Jupiter Engine));
	JPT_RETURN_FALSE_IF_ERROR(sourceStr != JPT_GET_PROPER_STRING(CharT, Jupiter Engine), "");

	StringT movedStr(jpt::Move(sourceStr));
	JPT_RETURN_FALSE_IF_ERROR(movedStr != JPT_GET_PROPER_STRING(CharT, Jupiter Engine), "");
	JPT_RETURN_FALSE_IF_ERROR(!sourceStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_ERROR(sourceStr.ConstBuffer() != nullptr, "");

	sourceStr = JPT_GET_PROPER_STRING(CharT, Reassigned String); 
	JPT_RETURN_FALSE_IF_ERROR(sourceStr != JPT_GET_PROPER_STRING(CharT, Reassigned String), "");
	JPT_RETURN_FALSE_IF_ERROR(movedStr != JPT_GET_PROPER_STRING(CharT, Jupiter Engine), "");

	movedStr = jpt::Move(sourceStr);
	JPT_RETURN_FALSE_IF_ERROR(!sourceStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_ERROR(sourceStr.ConstBuffer() != nullptr, "");
	JPT_RETURN_FALSE_IF_ERROR(movedStr != JPT_GET_PROPER_STRING(CharT, Reassigned String), "");

	movedStr += JPT_GET_PROPER_STRING(CharT, Further Testing);
	JPT_RETURN_FALSE_IF_ERROR(movedStr != JPT_GET_PROPER_STRING(CharT, Reassigned StringFurther Testing), "");

	sourceStr.MoveString(jpt::Move(movedStr));
	JPT_RETURN_FALSE_IF_ERROR(sourceStr != JPT_GET_PROPER_STRING(CharT, Reassigned StringFurther Testing), "");
	JPT_RETURN_FALSE_IF_ERROR(!movedStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_ERROR(movedStr.ConstBuffer() != nullptr, "");

	return true;
}

template<class StringT>
bool UnitTest_SubStr()
{
	using CharT = StringT::CharT;

	StringT sourceStr(JPT_GET_PROPER_STRING(CharT, Jupiter Engine));
	StringT subStr;

	subStr = sourceStr.SubStr(0, 7);
	JPT_RETURN_FALSE_IF_ERROR(subStr != JPT_GET_PROPER_STRING(CharT, Jupiter), "");

	subStr = sourceStr.SubStr(8, 6);
	JPT_RETURN_FALSE_IF_ERROR(subStr != JPT_GET_PROPER_STRING(CharT, Engine), "");

	return true;
}

template<class StringT>
bool UnitTest_ToString()
{
	using CharT = StringT::CharT;

	StringT string = jpt::ToString<StringT>(114151);
	JPT_RETURN_FALSE_IF_ERROR(string != JPT_GET_PROPER_STRING(CharT, 114151), "");

	string = jpt::ToString<StringT>(114151.114);
	JPT_RETURN_FALSE_IF_ERROR(string != JPT_GET_PROPER_STRING(CharT, 114151.114), "");

	// TODO [zshi]: Add containers, vector, and matrix when finished

	return true;
}

template<class StringT>
bool UnitTest_StringFinding()
{
	using CharT = StringT::CharT;
	StringT string(JPT_GET_PROPER_STRING(CharT, This is a test string for Jupiter Engine's jpt::BasicString'));

	size_t isPosition = string.Find(JPT_GET_PROPER_STRING(CharT, is));
	JPT_RETURN_FALSE_IF_ERROR(isPosition != 2, "");

	size_t ePosition = string.Find(JPT_GET_PROPER_STRING(CharT, e), 20);
	JPT_RETURN_FALSE_IF_ERROR(ePosition != 31, "");

	size_t lastIsPosition = string.FindLastOf(JPT_GET_PROPER_STRING(CharT, is));
	JPT_RETURN_FALSE_IF_ERROR(lastIsPosition != 5, "");

	size_t lastEPosition = string.FindLastOf(JPT_GET_PROPER_STRING(CharT, e));
	JPT_RETURN_FALSE_IF_ERROR(lastEPosition != 39, "");

	size_t last20EPosition = string.FindLastOf(JPT_GET_PROPER_STRING(CharT, e), 20, 33);
	JPT_RETURN_FALSE_IF_ERROR(last20EPosition != 31, "");

	size_t noExistString = string.Find(JPT_GET_PROPER_STRING(CharT, Doesnt Exist String));
	JPT_RETURN_FALSE_IF_ERROR(noExistString != StringT::npos, "");

	return true;
}

template<class StringT>
bool UnitTest_StringReplace()
{
	using CharT = StringT::CharT;
	StringT string(JPT_GET_PROPER_STRING(CharT, This is a test string for Jupiter Engine jpt::BasicString));

	string.Replace(JPT_GET_PROPER_STRING(CharT, e), JPT_GET_PROPER_STRING(CharT, LOL));
	JPT_RETURN_FALSE_IF_ERROR(string != JPT_GET_PROPER_STRING(CharT, This is a tLOLst string for JupitLOLr EnginLOL jpt::BasicString), "");
						
	string = JPT_GET_PROPER_STRING(CharT, This is a test string for Jupiter Engine jpt::BasicString);
	string.Replace(JPT_GET_PROPER_STRING(CharT, e), JPT_GET_PROPER_STRING(CharT, LOL), 20, 42);
	JPT_RETURN_FALSE_IF_ERROR(string != JPT_GET_PROPER_STRING(CharT, This is a test string for JupitLOLr EnginLOL jpt::BasicString), "");

	string = JPT_GET_PROPER_STRING(CharT, This is a test string for Jupiter Engine jpt::BasicString);
	string.Replace(JPT_GET_PROPER_STRING(CharT, e), JPT_GET_PROPER_STRING(CharT, LOL), 20, 32);
	JPT_RETURN_FALSE_IF_ERROR(string != JPT_GET_PROPER_STRING(CharT, This is a test string for JupitLOLr Engine jpt::BasicString), "");

	return true;
}

export bool RunStringUnitTests()
{
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_DefaultStringConstructing<jpt::String>(), "UnitTest_DefaultStringConstructing Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_DefaultStringConstructing<jpt::WString>(), "UnitTest_DefaultStringConstructing Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_ConvertingString<jpt::String>(), "UnitTest_ConvertingString Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_ConvertingString<jpt::WString>(), "UnitTest_ConvertingString Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_CopyingString<jpt::String>(), "UnitTest_ConvertingString Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_CopyingString<jpt::WString>(), "UnitTest_ConvertingString Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_MovingString<jpt::String>(), "UnitTest_MovingString Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_MovingString<jpt::WString>(), "UnitTest_MovingString Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_SubStr<jpt::String>(), "UnitTest_SubStr Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_SubStr<jpt::WString>(), "UnitTest_SubStr Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_ToString<jpt::String>(), "UnitTest_ToString Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_ToString<jpt::WString>(), "UnitTest_ToString Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StringFinding<jpt::String>(), "UnitTest_StringFinding Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StringFinding<jpt::WString>(), "UnitTest_StringFinding Failed");

	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StringReplace<jpt::String>(), "UnitTest_StringReplace Failed");
	JPT_RETURN_FALSE_IF_ERROR(!UnitTest_StringReplace<jpt::WString>(), "UnitTest_StringReplace Failed");

	return true;
}