// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <time.h>
#include <chrono>

export module jpt.DateTime;

import jpt.Timing.Utils;
import jpt.Timing.TypeDefs;
import jpt.DateTimePoint;

export namespace jpt
{
	DateTimePoint GetDateTime(const TimePoint<Clock>& point)
	{
		std::time_t time = Clock::to_time_t(point);
		std::tm t;
		localtime_s(&t, &time);
		return DateTimePoint(&t);
	}

	DateTimePoint GetCurrentDateTime()
	{
		return GetDateTime(Now());
	}
}