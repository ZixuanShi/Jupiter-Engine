// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <time.h>

export module jpt.TimePointData;

import jpt.TypeDefs;
import jpt.ToString;
import jpt.Limits;

export namespace jpt
{
	namespace Timing
	{
		struct PointData
		{
			uint32 year        : 12 = 0;    /**< 4095 */
			uint32 month       : 4  = 0;    /**< 15   */
			uint32 day         : 5  = 0;    /**< 31   */
			uint32 hour        : 5  = 0;    /**< 31   */
			uint32 minute      : 6  = 0;    /**< 63   */
			uint32 second      : 6  = 0;    /**< 63   */
			uint32 millisecond : 10 = 0;    /**< 1023 */

			constexpr PointData() = default;
			constexpr PointData(uint32 _year, uint32 _month, uint32 _day, uint32 _hour, uint32 _minute, uint32 _second, uint32 _millisecond);
			constexpr PointData(const tm* pTimeData);

			constexpr bool IsValid() const;
			constexpr String ToString() const;
		};

		constexpr PointData::PointData(uint32 _year, uint32 _month, uint32 _day, uint32 _hour, uint32 _minute, uint32 _second, uint32 _millisecond)
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

		constexpr PointData::PointData(const tm* pTimeData)
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

		constexpr bool PointData::IsValid() const
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

		constexpr String PointData::ToString() const
		{
			// MM/DD/YYYY. HH:MM:SS
			return String::Format<32>("%d/%d/%d. %d:%d:%d", month, day, year, hour, minute, second);
		}
	}

	template<>
	struct LimitsOf<Timing::PointData>
	{
		static constexpr Timing::PointData kMax =
		{
			4095,  // year  
			12,	   // month 
			31,	   // day   
			23,	   // hour  
			59,	   // minute
			59,	   // second
			999,   // millisecond
		};

		static constexpr Timing::PointData kMin =
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