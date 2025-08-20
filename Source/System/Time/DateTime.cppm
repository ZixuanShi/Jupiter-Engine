// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <time.h>
#include <chrono>

export module jpt.DateTime;

import jpt.TimeTypeDefs;
import jpt.TypeDefs;
import jpt.String;

export namespace jpt
{
    /** Used to represent date and time. Both a specific point and duration */
    class DateTime
    {
    public:
        // Directly changing values is not recommended. Use provided methods instead
        uint32 year   : 12 = 0;    /**< 4095. Range 0 - 4094 */
        uint32 month  : 4  = 0;    /**< 15    Range 0 - 12   */
        uint32 day    : 5  = 0;    /**< 31    Range 0 - 31   */
        uint32 hour   : 5  = 0;    /**< 31    Range 0 - 23   */
        uint32 minute : 6  = 0;    /**< 63    Range 0 - 59   */
        uint32 second : 6  = 0;    /**< 63    Range 0 - 59   */

    public:
        /** Constructors */
        DateTime() = default;
        DateTime(uint32 _year, uint32 _month, uint32 _day, uint32 _hour, uint32 _minute, uint32 _second);
        DateTime(const tm& timeData);
        DateTime(RawTimeType rawTime);
        DateTime(const std::chrono::time_point<std::chrono::system_clock>& clockPoint);

        /** Read data from different source */
        void ReadTM(const tm& timeData);
        void ReadRawTime(RawTimeType rawTime);
        void ReadClockPoint(const std::chrono::time_point<std::chrono::system_clock>& clockPoint);

        /** Convert to different source */
        tm ToTm() const;
        RawTimeType ToRawTime() const;
        std::chrono::time_point<std::chrono::system_clock> ToClockPoint() const;

        /** Modifiers. Pass in seconds to change date and time
            @example:

            // Get date time 10 years ago
            DateTime now = Clock::GetCurrentDateTime();
            now.Backward(60 * 60 * 24 * 365 * 10);        // 60 seconds * 60 minutes * 24 hours * 365 days * 10 = 10 years */
        void Forward(uint32 seconds);
        void Backward(uint32 seconds);

        /** Comparison */
        bool operator==(const DateTime& other) const;

        /** @return        Duration between this DateTime point to the other DateTime point */
        DateTime operator-(const DateTime& other) const;

        /** @return        The DateTime point relative to this DateTime point of duration */
        DateTime operator+(const DateTime& duration) const;

        /** Utilities */
        bool IsValid() const;
    };

    // ------------------------------------------------------------------------------------------------
    // Non-Member functions
    // ------------------------------------------------------------------------------------------------
    String ToString(const DateTime& dateTime)
    {
        // MM/DD/YYYY. HH:MM:SS
        return String::Format<32>("%d/%d/%d. %d:%d:%d", dateTime.month, dateTime.day, dateTime.year, dateTime.hour, dateTime.minute, dateTime.second);
    }

    String ToFileString(const DateTime& dateTime)
    {
        // MM_DD_YYYY_HH_MM_SS
        return String::Format<32>("%d_%d_%d_%d_%d_%d", dateTime.month, dateTime.day, dateTime.year, dateTime.hour, dateTime.minute, dateTime.second);
    }
}