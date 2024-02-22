// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <time.h>
#include <chrono>

export module jpt.StopWatch;

import jpt.Time.TypeDefs;
import jpt.Time.Utils;

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

		void Start();
		Precision GetDuration() const;
		Precision GetDurationMs() const;
	};

	StopWatch::Point StopWatch::Now()
	{
		return TClock::now();
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