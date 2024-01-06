// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include "Core/Building/Headers.h"

export module StringUnitTests;

import jpt.CoreModules;

bool UnitTest_DefaultStringConstructing()
{
	jpt::String defaultStr;
	JPT_RETURN_FALSE_IF_LOG(!defaultStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(defaultStr.ConstBuffer(), "", 0), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(defaultStr.Buffer(), "", 0), "");
	
	defaultStr.Reserve(30);
	JPT_RETURN_FALSE_IF_LOG(defaultStr.Capacity() != 30, "");
	JPT_RETURN_FALSE_IF_LOG(!defaultStr.IsEmpty(), "");

	defaultStr.Append("Jupiter Engine", 14);
	defaultStr.Append(" Zixuan", 7);
	JPT_RETURN_FALSE_IF_LOG(defaultStr.Capacity() != 30, "");
	JPT_RETURN_FALSE_IF_LOG(defaultStr != "Jupiter Engine Zixuan", "");

	defaultStr.Append(" Testing Buffer Resizing", 24);
	JPT_RETURN_FALSE_IF_LOG(defaultStr.Capacity() != 45, "");
	JPT_RETURN_FALSE_IF_LOG(defaultStr.Size() != 45, "");
	JPT_RETURN_FALSE_IF_LOG(defaultStr != "Jupiter Engine Zixuan Testing Buffer Resizing", "");

	defaultStr.Clear();
	JPT_RETURN_FALSE_IF_LOG(!defaultStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(defaultStr.ConstBuffer(), "", 0), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(defaultStr.Buffer(), "", 0), "");

	return true;
}

bool UnitTest_DefaultWStringConstructing()
{
	jpt::WString defaultStr;
	JPT_RETURN_FALSE_IF_LOG(!defaultStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(defaultStr.ConstBuffer(), L"", 0), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(defaultStr.Buffer(), L"", 0), "");

	defaultStr.Reserve(30);
	JPT_RETURN_FALSE_IF_LOG(defaultStr.Capacity() != 30, "");
	JPT_RETURN_FALSE_IF_LOG(!defaultStr.IsEmpty(), "");

	defaultStr.Append(L"Jupiter Engine", 14);
	defaultStr.Append(L" Zixuan", 7);
	JPT_RETURN_FALSE_IF_LOG(defaultStr.Capacity() != 30, "");
	JPT_RETURN_FALSE_IF_LOG(defaultStr != L"Jupiter Engine Zixuan", "");

	defaultStr.Append(L" Testing Buffer Resizing", 24);
	JPT_RETURN_FALSE_IF_LOG(defaultStr.Capacity() != 45, "");
	JPT_RETURN_FALSE_IF_LOG(defaultStr.Size() != 45, "");
	JPT_RETURN_FALSE_IF_LOG(defaultStr != L"Jupiter Engine Zixuan Testing Buffer Resizing", "");

	defaultStr.Clear();
	JPT_RETURN_FALSE_IF_LOG(!defaultStr.IsEmpty(), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(defaultStr.ConstBuffer(), L"", 0), "");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(defaultStr.Buffer(), L"", 0), "");

	return true;
}

bool UnitTest_ConvertingString()
{
	jpt::String numberStr("114514");
	JPT_RETURN_FALSE_IF_LOG(numberStr.ToInt() != 114514, "");

	numberStr = "114514.114514";
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreValuesClose(numberStr.ToFloat(), 114514.114514f), "");

	return true;
}

export void RunStringUnitTests()
{
	JPT_LOG_IF(!UnitTest_DefaultStringConstructing(), "UnitTest_DefaultStringConstructing Failed");
	JPT_LOG_IF(!UnitTest_DefaultWStringConstructing(), "UnitTest_DefaultWStringConstructing Failed");
	JPT_LOG_IF(!UnitTest_ConvertingString(), "UnitTest_ConvertingString Failed");
}