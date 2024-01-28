// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <chrono>

export module jpt.Timing.Utils;

export import jpt.Timing.TypeDefs;
import jpt.ToString;

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
			const Point<T> currentTimePoint = T::now();
			return GetSecondsBetween(start, currentTimePoint);
		}

		//template<BasicTStringype TString, ClockT T = Clock>
		//TString ToString(const Point<T>& point)
		//{
		//	return "TimePoint";
		//}
	}
}