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
		static Precision GetSecondsBetween(const Point& begin, const Point& end);
		static Precision GetSecondsFrom(const Point& begin);
		static Precision GetMsBetween(const Point& begin, const Point& end);
		static Precision GetMsFrom(const Point& begin);

		void Start();
		Precision GetDuration() const;
		Precision GetDurationMs() const;
	};

	StopWatch::Point StopWatch::Now()
	{
		return TClock::now();
	}

	Precision StopWatch::GetSecondsBetween(const Point& begin, const Point& end)
	{
		const std::chrono::duration<Precision> diff = end - begin;
		return diff.count();
	}

	Precision StopWatch::GetSecondsFrom(const Point& begin)
	{
		const auto end = Now();
		return GetSecondsBetween(begin, end);
	}

	Precision StopWatch::GetMsBetween(const Point& begin, const Point& end)
	{
		return GetSecondsBetween(begin, end) * static_cast<Precision>(1000.0f);
	}

	Precision StopWatch::GetMsFrom(const Point& begin)
	{
		return GetSecondsFrom(begin) * static_cast<Precision>(1000.0f);
	}

	void StopWatch::Start()
	{
		m_start = Now();
	}

	Precision StopWatch::GetDuration() const
	{
		return GetSecondsFrom(m_start);
	}

	Precision StopWatch::GetDurationMs() const
	{
		return GetMsFrom(m_start);
	}
}