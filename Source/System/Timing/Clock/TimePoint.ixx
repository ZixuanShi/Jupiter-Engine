// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

#include <time.h>

export module jpt.Clock.TimePoint;

import jpt.Clock.TimePointData;
import jpt.ToString;

export namespace jpt
{
	namespace Clock
	{
		/** Represents a point in time */
		class TimePoint
		{
		private:
			TimePointData m_data;

		public:
			constexpr TimePoint() = default;
			constexpr TimePoint(TimePointData data);
			constexpr TimePoint(const tm* pTimeData);

			constexpr String ToString() const;
		};

		constexpr TimePoint::TimePoint(TimePointData data)
			: m_data(data) 
		{
			JPT_ASSERT(m_data.IsValid());
		}

		constexpr TimePoint::TimePoint(const tm* pTimeData)
			: m_data(pTimeData)
		{
			JPT_ASSERT(m_data.IsValid());
		}

		constexpr String TimePoint::ToString() const
		{
			return m_data.ToString();
		}
	}
}