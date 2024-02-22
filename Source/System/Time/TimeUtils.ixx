// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <time.h>
#include <chrono>

export module jpt.Time.Utils;

import jpt.Time.TypeDefs;

export namespace jpt
{
	template<typename TimePoint>
	Precision GetSecondsBetween(const TimePoint& begin, const TimePoint& end)
	{
		const std::chrono::duration<Precision> diff = end - begin;
		return diff.count();
	}

	template<typename TimePoint>
	Precision GetSecondsFrom(const TimePoint& begin)
	{
		const auto end = TimePoint::clock::now();
		return GetSecondsBetween(begin, end);
	}

	template<typename TimePoint>
	Precision GetMsBetween(const TimePoint& begin, const TimePoint& end)
	{
		return GetSecondsBetween(begin, end) * static_cast<Precision>(1000.0f);
	}

	template<typename TimePoint>
	Precision GetMsFrom(const TimePoint& begin)
	{
		return GetSecondsFrom(begin) * static_cast<Precision>(1000.0f);
	}
}