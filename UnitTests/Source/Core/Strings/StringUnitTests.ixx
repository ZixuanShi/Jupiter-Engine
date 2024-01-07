// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Building/Headers.h"

export module StringUnitTests;

import jpt.CoreModules;

template<class StringType>
bool UnitTest_DefaultStringConstructing()
{
	using CharType = StringType::CharType;

	StringType defaultStr;
	JPT_RETURN_FALSE_IF_LOG(!defaultStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(defaultStr.ConstBuffer(), JPT_GET_PROPER_STRING(CharType, ), 0), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(defaultStr.Buffer(), JPT_GET_PROPER_STRING(CharType, ), 0), "");
	
	defaultStr.Reserve(30);
	JPT_RETURN_FALSE_IF_LOG(defaultStr.Capacity() != 30, "");
	JPT_RETURN_FALSE_IF_LOG(!defaultStr.IsEmpty(), "");

	defaultStr.Append(JPT_GET_PROPER_STRING(CharType, Jupiter Engine), 14);
	defaultStr.Append(JPT_GET_PROPER_STRING(CharType, Zixuan), 6);
	JPT_RETURN_FALSE_IF_LOG(defaultStr.Capacity() != 30, "");
	JPT_RETURN_FALSE_IF_LOG(defaultStr != JPT_GET_PROPER_STRING(CharType, Jupiter EngineZixuan), "");

	defaultStr.Append(JPT_GET_PROPER_STRING(CharType, Testing Buffer Resizing), 24);
	JPT_RETURN_FALSE_IF_LOG(defaultStr.Capacity() != 44, "");
	JPT_RETURN_FALSE_IF_LOG(defaultStr.Size() != 44, "");
	JPT_RETURN_FALSE_IF_LOG(defaultStr != JPT_GET_PROPER_STRING(CharType, Jupiter EngineZixuanTesting Buffer Resizing), "");

	defaultStr.Clear();
	JPT_RETURN_FALSE_IF_LOG(!defaultStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(defaultStr.ConstBuffer(), JPT_GET_PROPER_STRING(CharType, ), 0), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(defaultStr.Buffer(), JPT_GET_PROPER_STRING(CharType, ), 0), "");

	return true;
}

template<class StringType>
bool UnitTest_ConvertingString()
{
	using CharType = StringType::CharType;

	StringType numberStr("114514");
	JPT_RETURN_FALSE_IF_LOG(numberStr.ToInt() != 114514, "");

	numberStr = "114514.114514";
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreValuesClose(numberStr.ToFloat(), 114514.114514f), "");

	return true;
}

bool UnitTest_CopyingString()
{
	jpt::String sourceStr("Jupiter Engine");
	JPT_RETURN_FALSE_IF_LOG(sourceStr != "Jupiter Engine", "");

	jpt::String copiedStr(sourceStr);
	JPT_RETURN_FALSE_IF_LOG(copiedStr != "Jupiter Engine", "");
	JPT_RETURN_FALSE_IF_LOG(sourceStr != copiedStr, "");

	sourceStr = "Reassigned String";
	JPT_RETURN_FALSE_IF_LOG(sourceStr != "Reassigned String", "");
	JPT_RETURN_FALSE_IF_LOG(copiedStr != "Jupiter Engine", "");

	copiedStr = sourceStr;
	JPT_RETURN_FALSE_IF_LOG(sourceStr != "Reassigned String", "");
	JPT_RETURN_FALSE_IF_LOG(copiedStr != "Reassigned String", "");

	copiedStr += " Further Testing";
	JPT_RETURN_FALSE_IF_LOG(copiedStr != "Reassigned String Further Testing", "");

	sourceStr.CopyString(copiedStr);
	JPT_RETURN_FALSE_IF_LOG(sourceStr != "Reassigned String Further Testing", "");

	return true;
}

template<class StringType>
bool UnitTest_MovingString()
{
	using CharType = StringType::CharType;

	StringType sourceStr(JPT_GET_PROPER_STRING(CharType, Jupiter Engine));
	JPT_RETURN_FALSE_IF_LOG(sourceStr != JPT_GET_PROPER_STRING(CharType, Jupiter Engine), "");

	StringType movedStr(jpt::move(sourceStr));
	JPT_RETURN_FALSE_IF_LOG(movedStr != JPT_GET_PROPER_STRING(CharType, Jupiter Engine), "");
	JPT_RETURN_FALSE_IF_LOG(!sourceStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_LOG(sourceStr.ConstBuffer() != nullptr, "");

	sourceStr = JPT_GET_PROPER_STRING(CharType, Reassigned String); 
	JPT_RETURN_FALSE_IF_LOG(sourceStr != JPT_GET_PROPER_STRING(CharType, Reassigned String), "");
	JPT_RETURN_FALSE_IF_LOG(movedStr != JPT_GET_PROPER_STRING(CharType, Jupiter Engine), "");

	movedStr = jpt::move(sourceStr);
	JPT_RETURN_FALSE_IF_LOG(!sourceStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_LOG(sourceStr.ConstBuffer() != nullptr, "");
	JPT_RETURN_FALSE_IF_LOG(movedStr != JPT_GET_PROPER_STRING(CharType, Reassigned String), "");

	movedStr += JPT_GET_PROPER_STRING(CharType, Further Testing);
	JPT_RETURN_FALSE_IF_LOG(movedStr != JPT_GET_PROPER_STRING(CharType, Reassigned StringFurther Testing), "");

	sourceStr.MoveString(jpt::move(movedStr));
	JPT_RETURN_FALSE_IF_LOG(sourceStr != JPT_GET_PROPER_STRING(CharType, Reassigned StringFurther Testing), "");
	JPT_RETURN_FALSE_IF_LOG(!movedStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_LOG(movedStr.ConstBuffer() != nullptr, "");

	return true;
}

export void RunStringUnitTests()
{
	JPT_LOG_IF(!UnitTest_DefaultStringConstructing<jpt::String>(), "UnitTest_DefaultStringConstructing Failed");
	JPT_LOG_IF(!UnitTest_DefaultStringConstructing<jpt::WString>(), "UnitTest_DefaultStringConstructing Failed");

	JPT_LOG_IF(!UnitTest_ConvertingString<jpt::String>(), "UnitTest_ConvertingString Failed");

	JPT_LOG_IF(!UnitTest_CopyingString(), "UnitTest_ConvertingString Failed");

	JPT_LOG_IF(!UnitTest_MovingString<jpt::String>(), "UnitTest_MovingString Failed");
	JPT_LOG_IF(!UnitTest_MovingString<jpt::WString>(), "UnitTest_MovingString Failed");



	//JPT_LOG_IF(!UnitTest_SubStr(), "UnitTest_SubStr Failed");
}