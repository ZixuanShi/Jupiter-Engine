// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <time.h>
#include <chrono>

export module jpt.Clock;

import jpt.Time.TypeDefs;

export namespace jpt
{
	/** Use for simulating real-life time. Getting specific Year, Month, Day, Hour, Minute, Second */
	class Clock
	{
	public:
		using TClock = std::chrono::system_clock;
		using Point  = std::chrono::time_point<TClock>;

	public:
		static Point Now() { return TClock::now(); }
	};
}