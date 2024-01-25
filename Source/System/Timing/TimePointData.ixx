// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.TimePointData;

import jpt.TypeDefs;
import jpt.ToString;
import jpt.NumericLimits;

export namespace jpt
{
	struct TimePointData
	{
		uint32 year        : 12 = 0;     /**< 4095 */
		uint32 month       : 4  = 0;     /**< 15 */
		uint32 day         : 5  = 0;     /**< 31 */
		uint32 hour        : 5  = 0;     /**< 31 */
		uint32 minute      : 6  = 0;     /**< 63 */
		uint32 second      : 6  = 0;     /**< 63 */
		uint32 millisecond : 10 = 0;     /**< 1023 */

		constexpr TimePointData() = default;
		constexpr TimePointData(uint32 _year, uint32 _month, uint32 _day, uint32 _hour, uint32 _minute, uint32 _second, uint32 _millisecond)
			: year(_year)
			, month(_month)
			, day(_day)
			, hour(_hour)
			, minute(_minute)
			, second(_second)
			, millisecond(_millisecond)
		{
		}

		String ToString() const
		{
			return String::Format<32>("%d/%d/%d. %d:%d:%d", year, month, day, hour, minute, second);
		}
	};

	template<>
	struct LimitsOf<TimePointData>
	{
		static constexpr TimePointData kMax =
		{
			4095,  // year  
			12,	   // month 
			31,	   // day   
			23,	   // hour  
			59,	   // minute
			59,	   // second
			1000,  // millisecond
		};

		static constexpr TimePointData kMin =
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