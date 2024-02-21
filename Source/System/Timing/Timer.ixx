// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <time.h>
#include <chrono>

export module jpt.Timer;

import jpt.Timing.Utils;
import jpt.Timing.TypeDefs;

namespace jpt
{
	export class Timer
	{
	private:
		TimePoint<StopWatch> m_start;

	public:
		void Start();

		Precision GetElapsed() const;
		Precision GetElapsedMs() const;
	};

	void Timer::Start()
	{
		m_start = Now<StopWatch>();
	}

	Precision Timer::GetElapsed() const
	{
		return GetSecondsFrom(m_start);
	}

	Precision Timer::GetElapsedMs() const
	{
		return GetMilliSecondsFrom(m_start);
	}
}