// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

export module UnitTests_Clock;

import jpt.CoreModules;
import jpt.Clock;

bool UnitTest_Clock()
{
    JPT_LOG(jpt::Clock::Now());

    return true;
}

export bool RunUnitTests_Clock()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Clock(), "UnitTest_Clock Failed");

	JPT_LOG(jpt::LimitsOf<jpt::Clock::TimePointData>::kMax);
	JPT_LOG(jpt::LimitsOf<jpt::Clock::TimePointData>::kMin);

	jpt::Clock::TimePointData timeInfo;
	timeInfo.year   = 1999;
	timeInfo.month  = 7;
	timeInfo.day    = 26;
	timeInfo.hour   = 5;
	timeInfo.minute = 30;
	timeInfo.second = 1;
	JPT_LOG(jpt::Clock::TimePoint(timeInfo));

	timeInfo.year   = 1999;
	timeInfo.month  = 12;
	timeInfo.day    = 31;
	timeInfo.hour   = 23;
	timeInfo.minute = 59;
	timeInfo.second = 29;
	JPT_LOG(jpt::Clock::TimePoint(timeInfo));

    return true;
}
