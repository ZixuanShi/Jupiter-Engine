// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module; 

#include <time.h>
#include <chrono>

export module jpt.Timing.Utils;

export import jpt.Timing.TypeDefs;
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
			const Point<T> currentTimePoint = T::now();
			return GetSecondsBetween(start, currentTimePoint);
		}
	}

	String ToString(const std::chrono::time_point<std::chrono::system_clock>& point)
	{
		static constexpr size_t kStringSize = 32;

		std::time_t time = std::chrono::system_clock::to_time_t(point);
		std::tm t;
		localtime_s(&t, &time);

		char buffer[kStringSize];
		strftime(buffer, sizeof(buffer), "%m/%d/%Y. %X", &t);

		jpt::String timeString;
		timeString.CopyString(buffer, kStringSize);
		return timeString;
	}
}