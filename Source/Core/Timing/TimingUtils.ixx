// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <chrono>

export module jpt.TimingUtils;

import jpt.String;

export namespace jpt
{
	// Concepts
	template<class T>
	concept ClockT = std::chrono::is_clock_v<T>;

	// TDefs
	using TimingPrecision = double;
	using TimeDuration = std::chrono::duration<TimingPrecision>; /**< Duration between two TimePoints */

	using SteadyClock = std::chrono::steady_clock;				/**< Stopwatch that will only grow positively. Use for timing and benchmarking. */
	using SystemClock = std::chrono::system_clock;				/**< Regular system-wide realtime clock. Use for simulating real-life time like day/night shifting*/
	using HighResolutionClock = std::chrono::high_resolution_clock;		/**< The clock with the shortest tick period and highest precision available. */

	/**< Represents a point in time. It is implemented as if it stores a value of T Duration indicating the time interval from the start of the Clock's epoch. */
	template<ClockT Clock>
	using TimePoint = std::chrono::time_point<Clock>;

	/** @return Duration in seconds between two time points from start to end */
	template<ClockT Clock>
	TimingPrecision GetSecondsBetween(const TimePoint<Clock>& start, const TimePoint<Clock>& end)
	{
		const TimeDuration duration = std::chrono::duration_cast<TimeDuration>(end - start);
		return duration.count();
	}

	/** @return Duration in seconds between two time points from start to now */
	template<ClockT Clock>
	TimingPrecision GetSecondsFrom(const TimePoint<Clock>& start)
	{
		const TimePoint<Clock> currentTimePoint = SteadyClock::now();
		return GetSecondsBetween(start, currentTimePoint);
	}

	String ToString(const TimePoint<SystemClock>& timePoint)
	{
		std::time_t time = SystemClock::to_time_t(timePoint);
		std::tm t;
		localtime_s(&t, &time);
		char buffer[32];
		strftime(buffer, sizeof(buffer), "%m/%d/%Y %X", &t);

		jpt::String timeString;
		timeString.CopyString(buffer);
		return timeString;
	}
}