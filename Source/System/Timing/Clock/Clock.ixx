// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <chrono>
#include <time.h>

export module jpt.Clock;

import jpt.Clock.TimePoint;
import jpt.TypeDefs;

/** Regular system-wide realtime clock. 
	Use for simulating real-life time like day/night shifting*/
export namespace jpt::Clock
{
	/** @return		TimePoint that represents the current world-clock time */
	TimePoint Now()
	{
		const auto now = std::chrono::system_clock::now();
		const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
		tm* timeinfo = new tm;
		localtime_s(timeinfo, &t_c);

		return TimePoint(timeinfo);
	}
}