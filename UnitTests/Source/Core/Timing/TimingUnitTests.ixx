// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module TimingUnitTests;

import jpt.CoreModules;

bool UnitTest_Timing()
{
	//jpt::TimePointParam data;
	//data.year = 1999;
	//data.month = 7;
	//data.day = 26;
	//data.hour = 5;
	//data.minute = 0;
	//data.second = 30;
	//jpt::TimePoint timePoint(data);


	// timePoint = SystemClock::now();	// SystemClock::now() contains current TimePointParam



	return true;
}

export bool RunTimingUnitTests()
{
	//jpt::TimePoint timeInfo;
	//JPT_LOG("sizeof TimePoint %d", sizeof(timeInfo));

	JPT_LOG(jpt::NumericLimits<jpt::TimePoint>::kMax);
	JPT_LOG(jpt::NumericLimits<jpt::TimePoint>::kMin);

	//timeInfo.year   = 1999;
	//timeInfo.month  = 7;
	//timeInfo.day    = 26;
	//timeInfo.hour   = 5;
	//timeInfo.minute = 30;
	//timeInfo.second = 1;
	//JPT_LOG(timeInfo);

	//timeInfo.year   = 1999;
	//timeInfo.month  = 12;
	//timeInfo.day    = 31;
	//timeInfo.hour   = 24;
	//timeInfo.minute = 60;
	//timeInfo.second = 60;
	//JPT_LOG(timeInfo);

	//timeInfo.year   = 1999;
	//timeInfo.month  = 13;
	//timeInfo.day    = 32;
	//timeInfo.hour   = 25;
	//timeInfo.minute = 61;
	//timeInfo.second = 61;
	//JPT_LOG(timeInfo);

	return true;
}