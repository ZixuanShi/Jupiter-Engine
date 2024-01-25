// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <chrono>
#include <time.h>

export module jpt.Clock;

import jpt.TimePoint;

export namespace jpt::Clock
{
	/** @return		TimePoint that represents the current world-clock time */
	TimePoint Now()
	{
		//std::chrono::system_clock::now();
		return TimePoint();
	}
}