// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <time.h>
#include <chrono>

export module jpt.DateTime;

import jpt.Timing.Utils;
import jpt.Timing.TypeDefs;
import jpt.TimePointData;

export namespace jpt
{
	Timing::PointData GetDateTime(const std::chrono::time_point<Timing::Clock>& point)
	{
		std::time_t time = Timing::Clock::to_time_t(point);
		std::tm t;
		localtime_s(&t, &time);
		return Timing::PointData(&t);
	}

	Timing::PointData GetCurrentDateTime()
	{
		return GetDateTime(Timing::Now());
	}
}