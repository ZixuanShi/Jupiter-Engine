// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Building/Headers.h"

export module StringUtilsUnitTests;

import jpt.CoreModules;

bool UnitTest_StringLen()
{
	JPT_RETURN_FALSE_IF_LOG(jpt::GetStrLength("Jupiter Engine") != 14, "");
	JPT_RETURN_FALSE_IF_LOG(jpt::GetStrLength("Zixuan Shi") != 10, "");

	JPT_RETURN_FALSE_IF_LOG(jpt::GetStrLength(L"Jupiter Engine") != 14, "");
	JPT_RETURN_FALSE_IF_LOG(jpt::GetStrLength(L"Zixuan Shi") != 10, "");
	return true;
}

bool UnitTest_ToCStr()
{
	const char* integerCStr = jpt::IntegerToCStr(114514);
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(integerCStr, "114514", 6), "");
	delete integerCStr;

	integerCStr = jpt::IntegerToCStr(-114514);
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(integerCStr, "-114514", 7), "");
	delete integerCStr;

	const char* floatingCStr = jpt::FloatingToCStr(-114514.114f);
	//JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(floatingCStr, "-114514.114", 11), "");	// Not stable
	delete floatingCStr;

	return true;
}

bool UnitTest_StrCpy()
{
	char buffer[256];
	jpt::StrCpy(buffer, 15, "Jupiter Engine");
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(buffer, "Jupiter Engine", 14), "");

	jpt::StrNCpy(buffer, 15, "Jupiter Engine", 10);
	JPT_RETURN_FALSE_IF_LOG(!jpt::AreStringsSame(buffer, "Jupiter En", 10), "");

	return true;
}

export void RunStringUtilsUnitTests()
{
	JPT_LOG_IF(!UnitTest_StringLen(), "UnitTest_StringLen Failed");
	JPT_LOG_IF(!UnitTest_ToCStr(), "UnitTest_ToCStr Failed");
	JPT_LOG_IF(!UnitTest_StrCpy(), "UnitTest_ToCStr Failed");
}