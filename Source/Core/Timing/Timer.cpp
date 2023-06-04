// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "Timer.h"

#include <chrono>

namespace jpt
{
	TimePoint locTimePoint;

	void Timer::Start()
	{
		locTimePoint = Clock::now();
	}

	double Timer::GetTimeDuration()
	{
		const TimePoint now = Clock::now();
		const TimeDuration timeDuration = std::chrono::duration_cast<TimeDuration>(now - locTimePoint);
		return timeDuration.count() * 1'000;	// Convert to milliseconds
	}
}
