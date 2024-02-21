// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <time.h>
#include <chrono>

export module jpt.Timer;

import jpt.Timing.Utils;
import jpt.Timing.TypeDefs;

namespace jpt
{
	using namespace Timing;

	export class Timer
	{
		using TClock = StopWatch;
		using Point = Point<TClock>;

	private:
		Point m_start;

	public:
		void Start();

		Precision GetElapsed() const;
		Precision GetElapsedMs() const;
	};

	void Timer::Start()
	{
		m_start = Now<TClock>();
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