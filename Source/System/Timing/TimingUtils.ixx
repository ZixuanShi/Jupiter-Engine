// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <time.h>
#include <chrono>

export module jpt.Timing.Utils;

import jpt.Timing.TypeDefs;
import jpt.String;

export namespace jpt
{
	namespace Timing
	{
		template<ClockT T = Clock>
		constexpr Point<T> Now()
		{
			return T::now();
		}

		/** @return Duration in seconds between two time points from start to end */
		template<ClockT T>
		constexpr Precision GetSecondsBetween(const Point<T>& start, const Point<T>& end)
		{
			const Duration duration = std::chrono::duration_cast<Duration>(end - start);
			return duration.count();
		}

		/** @return Duration in seconds between two time points from start to now */
		template<ClockT T>
		constexpr Precision GetSecondsFrom(const Point<T>& start)
		{
			const Point<T> currentTimePoint = Now<T>();
			return GetSecondsBetween(start, currentTimePoint);
		}

		/** @return Duration in milli-seconds between two time points from start to end */
		template<ClockT T>
		constexpr Precision GetMilliSecondsBetween(const Point<T>& start, const Point<T>& end)
		{
			const Duration duration = std::chrono::duration_cast<Duration>(end - start);
			return duration.count() * static_cast<Precision>(1000.0f);
		}

		/** @return Duration in seconds between two time points from start to now */
		template<ClockT T>
		constexpr Precision GetMilliSecondsFrom(const Point<T>& start)
		{
			const Point<T> currentTimePoint = Now<T>();
			return GetMilliSecondsBetween(start, currentTimePoint);
		}
	}
}