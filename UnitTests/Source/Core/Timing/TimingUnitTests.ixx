// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module TimingUnitTests;

import jpt.CoreModules;

// Timing
export import jpt.TimePoint;
export import jpt.TimePointData;
export import jpt.Clock;
export import jpt.TimingUtilsDeprecated;

bool UnitTest_Timing()
{

	return true;
}

export bool RunTimingUnitTests()
{
	JPT_LOG(jpt::LimitsOf<jpt::TimePointData>::kMax);
	JPT_LOG(jpt::LimitsOf<jpt::TimePointData>::kMin);

	jpt::TimePointData timeInfo;
	timeInfo.year   = 1999;
	timeInfo.month  = 7;
	timeInfo.day    = 26;
	timeInfo.hour   = 5;
	timeInfo.minute = 30;
	timeInfo.second = 1;
	JPT_LOG(timeInfo);

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