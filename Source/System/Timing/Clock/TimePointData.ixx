// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimals/Headers.h"

#include <time.h>

export module jpt.Clock.TimePointData;

import jpt.TypeDefs;
import jpt.ToString;
import jpt.Limits;

export namespace jpt
{
	namespace Clock
	{
		struct TimePointData
		{
			uint32 year   : 12 = 0;    /**< 4095 */
			uint32 month  : 4  = 0;    /**< 15 */
			uint32 day    : 5  = 0;    /**< 31 */
			uint32 hour   : 5  = 0;    /**< 31 */
			uint32 minute : 6  = 0;    /**< 63 */
			uint32 second : 6  = 0;    /**< 63 */

			constexpr TimePointData() = default;
			constexpr TimePointData(uint32 _year, uint32 _month, uint32 _day, uint32 _hour, uint32 _minute, uint32 _second);
			constexpr TimePointData(const tm* pTimeData);

			constexpr bool IsValid() const;

			String ToString() const;
		};

		constexpr TimePointData::TimePointData(uint32 _year, uint32 _month, uint32 _day, uint32 _hour, uint32 _minute, uint32 _second)
			: year  (_year  )
			, month (_month )
			, day   (_day   )
			, hour  (_hour  )
			, minute(_minute)
			, second(_second)
		{
			JPT_ASSERT(IsValid());
		}

		constexpr TimePointData::TimePointData(const tm* pTimeData)
			: year  (pTimeData->tm_year + 1900)
			, month (pTimeData->tm_mon  + 1   )
			, day   (pTimeData->tm_mday       )
			, hour  (pTimeData->tm_hour       )
			, minute(pTimeData->tm_min        )
			, second(pTimeData->tm_sec        )
		{
			JPT_ASSERT(IsValid());
			JPT_DELETE(pTimeData);
		}

		constexpr bool TimePointData::IsValid() const
		{
			const bool validYear    = year    >= 1 && year   <= 4095;
			const bool validMonth   = month   >= 1 && month  <= 12;
			const bool validDay     = day     >= 1 && day    <= 31;
			const bool validHour    = hour    >= 0 && hour   <= 23;
			const bool validMinute  = minute  >= 0 && minute <= 59;
			const bool validSecond  = second  >= 0 && second <= 59;

			return validYear && validMonth  && validDay    &&
				   validHour && validMinute && validSecond;
		}

		String TimePointData::ToString() const
		{
			// MM/DD/YYYY. HH:MM:SS
			return String::Format<32>("%d/%d/%d. %d:%d:%d", month, day, year, hour, minute, second);
		}
	}

	template<>
	struct LimitsOf<Clock::TimePointData>
	{
		static constexpr Clock::TimePointData kMax =
		{
			4095,  // year  
			12,	   // month 
			31,	   // day   
			23,	   // hour  
			59,	   // minute
			59,	   // second
		};

		static constexpr Clock::TimePointData kMin =
		{
			1,  // year  
			1,  // month 
			1,  // day   
			0,  // hour  
			0,  // minute
			0,  // second
		};
	};
}