// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.TimePoint;

import jpt.TypeDefs;
import jpt.ToString;
import jpt.NumericLimits;

export namespace jpt
{
	/** Represents a point in time */
	struct TimePoint
	{
		uint32 year   :12  = 0;     /**< 4095 */
		uint32 month  : 4  = 0;     /**< 15 */
		uint32 day    : 5  = 0;     /**< 31 */
		uint32 hour   : 5  = 0;     /**< 31 */
		uint32 minute : 6  = 0;     /**< 63 */
		uint32 second : 6  = 0;     /**< 63 */

		/** @return		A string showing time point info with the default format of "MM/DD/YYYY HH:MM:SS" */
		String ToString() const;
	};

	String TimePoint::ToString() const
	{
		return String::Format<32>("%d/%d/%d %d:%d:%d", year, month, day, hour, minute, second);
	}

	template<>
	struct NumericLimits<TimePoint>
	{
		static constexpr TimePoint kMax = 
		{
			4095,  // year  
			12,	   // month 
			31,	   // day   
			23,	   // hour  
			59,	   // minute
			59,	   // second
		};

		static constexpr TimePoint kMin = {};
	};
}