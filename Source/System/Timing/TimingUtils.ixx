// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <time.h>
#include <chrono>

export module jpt.Timing.Utils;

import jpt.Timing.TypeDefs;
import jpt.String;

export namespace jpt
{
	template<ClockT T = Clock>
	constexpr TimePoint<T> Now()
	{
		return T::now();
	}

	/** @return Duration in seconds between two time points from start to end */
	template<ClockT T = Clock>
	constexpr Precision GetSecondsBetween(const TimePoint<T>& start, const TimePoint<T>& end)
	{
		const Duration duration = std::chrono::duration_cast<Duration>(end - start);
		return duration.count();
	}

	/** @return Duration in seconds between two time points from start to now */
	template<ClockT T = Clock>
	constexpr Precision GetSecondsFrom(const TimePoint<T>& start)
	{
		const TimePoint<T> currentTimePoint = Now<T>();
		return GetSecondsBetween(start, currentTimePoint);
	}

	/** @return Duration in milli-seconds between two time points from start to end */
	template<ClockT T = Clock>
	constexpr Precision GetMilliSecondsBetween(const TimePoint<T>& start, const TimePoint<T>& end)
	{
		const Duration duration = std::chrono::duration_cast<Duration>(end - start);
		return duration.count() * static_cast<Precision>(1000.0f);
	}

	/** @return Duration in seconds between two time points from start to now */
	template<ClockT T = Clock>
	constexpr Precision GetMilliSecondsFrom(const TimePoint<T>& start)
	{
		const TimePoint<T> currentTimePoint = Now<T>();
		return GetMilliSecondsBetween(start, currentTimePoint);
	}
}