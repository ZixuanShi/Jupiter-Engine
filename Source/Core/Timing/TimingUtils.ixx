// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <chrono>

export module jpt.TimingUtils;

export namespace jpt
{
	// Concepts
	template<class Type>
	concept ClockType = std::chrono::is_clock_v<Type>;

	// TypeDefs
	using TimingPrecision = double;
	using TimeDuration = std::chrono::duration<TimingPrecision>; /**< Duration between two TimePoints */

	using SteadyClock = std::chrono::steady_clock;				/**< Stopwatch that will only grow positively. Use for timing and benchmarking. */
	using SystemClock = std::chrono::system_clock;				/**< Regular system-wide realtime clock. Use for simulating real-life time like day/night shifting*/
	using HighResolutionClock = std::chrono::high_resolution_clock;		/**< The clock with the shortest tick period and highest precision available. */

	/**< Represents a point in time.It is implemented as if it stores a value of type Duration indicating the time interval from the start of the Clock's epoch. */
	template<ClockType Clock>
	using TimePoint = std::chrono::time_point<Clock>;

	/** @return Duration in seconds between two time points from start to end */
	template<ClockType Clock>
	TimingPrecision GetSecondsBetween(const TimePoint<Clock>& start, const TimePoint<Clock>& end)
	{
		const TimeDuration duration = std::chrono::duration_cast<TimeDuration>(end - start);
		return duration.count();
	}

	/** @return Duration in seconds between two time points from start to now */
	template<ClockType Clock>
	TimingPrecision GetSecondsFrom(const TimePoint<Clock>& start)
	{
		const TimePoint<Clock> currentTimePoint = SteadyClock::now();
		return GetSecondsBetween(start, currentTimePoint);
	}
}