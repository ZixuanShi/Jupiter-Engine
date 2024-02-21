// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <time.h>

export module jpt.DateTimePoint;

import jpt.TypeDefs;
import jpt.ToString;
import jpt.String;
import jpt.Limits;

export namespace jpt
{
	struct DateTimePoint
	{
		uint32 year        : 12 = 0;    /**< 4095 */
		uint32 month       : 4  = 0;    /**< 15   */
		uint32 day         : 5  = 0;    /**< 31   */
		uint32 hour        : 5  = 0;    /**< 31   */
		uint32 minute      : 6  = 0;    /**< 63   */
		uint32 second      : 6  = 0;    /**< 63   */
		uint32 millisecond : 10 = 0;    /**< 1023 */

		constexpr DateTimePoint() = default;
		constexpr DateTimePoint(uint32 _year, uint32 _month, uint32 _day, uint32 _hour, uint32 _minute, uint32 _second, uint32 _millisecond);
		constexpr DateTimePoint(const tm* pTimeData);

		constexpr bool IsValid() const;
		String ToString() const;
		tm ToTm() const;
	};

	constexpr DateTimePoint::DateTimePoint(uint32 _year, uint32 _month, uint32 _day, uint32 _hour, uint32 _minute, uint32 _second, uint32 _millisecond)
		: year       (_year       )
		, month      (_month      )
		, day        (_day        )
		, hour       (_hour       )
		, minute     (_minute     )
		, second     (_second     )
		, millisecond(_millisecond)
	{
		JPT_ASSERT(IsValid());
	}

	constexpr DateTimePoint::DateTimePoint(const tm* pTimeData)
		: year  (pTimeData->tm_year + 1900)
		, month (pTimeData->tm_mon  + 1   )
		, day   (pTimeData->tm_mday       )
		, hour  (pTimeData->tm_hour       )
		, minute(pTimeData->tm_min        )
		, second(pTimeData->tm_sec        )
	{
		JPT_ASSERT(IsValid());
	}

	constexpr bool DateTimePoint::IsValid() const
	{
		const bool validYear        = year        >= 1 && year        <= 4095;
		const bool validMonth       = month       >= 1 && month       <= 12;
		const bool validDay         = day         >= 1 && day         <= 31;
		const bool validHour        = hour        >= 0 && hour        <= 23;
		const bool validMinute      = minute      >= 0 && minute      <= 59;
		const bool validSecond      = second      >= 0 && second      <= 59;
		const bool validMilliSecond = millisecond >= 0 && millisecond <= 999;

		return validYear && validMonth  && validDay    &&
			   validHour && validMinute && validSecond && validMilliSecond;
	}

	String DateTimePoint::ToString() const
	{
		static constexpr size_t kStringSize = 32;

		const tm t = ToTm();
		// MM/DD/YYYY. HH:MM:SS
		char buffer[kStringSize];
		strftime(buffer, sizeof(buffer), "%m/%d/%Y. %X", &t);

		jpt::String timeString;
		timeString.CopyString(buffer, kStringSize);
		return timeString;
	}

	tm DateTimePoint::ToTm() const
	{
		tm timeData = {};
		timeData.tm_year = year - 1900;
		timeData.tm_mon  = month - 1;
		timeData.tm_mday = day;
		timeData.tm_hour = hour;
		timeData.tm_min  = minute;
		timeData.tm_sec  = second;

		return timeData;
	}

	template<>
	struct LimitsOf<DateTimePoint>
	{
		static constexpr DateTimePoint kMax =
		{
			4095,  // year  
			12,	   // month 
			31,	   // day   
			23,	   // hour  
			59,	   // minute
			59,	   // second
			999,   // millisecond
		};

		static constexpr DateTimePoint kMin =
		{
			1,  // year  
			1,  // month 
			1,  // day   
			0,  // hour  
			0,  // minute
			0,  // second
			0,  // millisecond
		};
	};
}