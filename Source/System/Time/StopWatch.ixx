// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <time.h>
#include <chrono>

export module jpt.StopWatch;

import jpt.Time.TypeDefs;

export namespace jpt
{
	/** Use for timing and benchmarking */
	class StopWatch
	{
		using Clock = std::chrono::high_resolution_clock;

	public:
		using Point = std::chrono::time_point<Clock>;

	public:
		static Point Now();

		static Precision GetSecondsBetween(const Point& begin, const Point& end);
		static Precision GetSecondsFrom(const Point& begin);

		static Precision GetMsBetween(const Point& begin, const Point& end);
		static Precision GetMsFrom(const Point& begin);
	};

	StopWatch::Point StopWatch::Now()
	{
		return Clock::now();
	}

	Precision StopWatch::GetSecondsBetween(const Point& begin, const Point& end)
	{
		const std::chrono::duration<Precision> diff = end - begin;
		return diff.count();
	}

	Precision StopWatch::GetSecondsFrom(const Point& begin)
	{
		const auto end = Now();
		const std::chrono::duration<Precision> diff = end - begin;
		return diff.count();
	}

	Precision StopWatch::GetMsBetween(const Point& begin, const Point& end)
	{
		return GetSecondsBetween(begin, end) * static_cast<Precision>(1000.0f);
	}

	Precision StopWatch::GetMsFrom(const Point& begin)
	{
		return GetSecondsFrom(begin) * static_cast<Precision>(1000.0f);
	}
}