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
	public:
		using TClock = std::chrono::high_resolution_clock;
		using Point  = std::chrono::time_point<TClock>;

	private:
		Point m_start;

	public:
		static Point Now();
		static TimePrecision GetSecondsBetween(const Point& begin, const Point& end);
		static TimePrecision GetSecondsFrom(const Point& begin);
		static TimePrecision GetMsBetween(const Point& begin, const Point& end);
		static TimePrecision GetMsFrom(const Point& begin);

		void Start();
		TimePrecision GetDuration() const;
		TimePrecision GetDurationMs() const;
	};

	StopWatch::Point StopWatch::Now()
	{
		return TClock::now();
	}

	TimePrecision StopWatch::GetSecondsBetween(const Point& begin, const Point& end)
	{
		const std::chrono::duration<TimePrecision> diff = end - begin;
		return diff.count();
	}

	TimePrecision StopWatch::GetSecondsFrom(const Point& begin)
	{
		const auto end = Now();
		return GetSecondsBetween(begin, end);
	}

	TimePrecision StopWatch::GetMsBetween(const Point& begin, const Point& end)
	{
		return GetSecondsBetween(begin, end) * static_cast<TimePrecision>(1000.0f);
	}

	TimePrecision StopWatch::GetMsFrom(const Point& begin)
	{
		return GetSecondsFrom(begin) * static_cast<TimePrecision>(1000.0f);
	}

	void StopWatch::Start()
	{
		m_start = Now();
	}

	TimePrecision StopWatch::GetDuration() const
	{
		return GetSecondsFrom(m_start);
	}

	TimePrecision StopWatch::GetDurationMs() const
	{
		return GetMsFrom(m_start);
	}
}