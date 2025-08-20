// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <time.h>
#include <chrono>

module jpt.DateTime;

namespace jpt
{
    DateTime::DateTime(uint32 _year, uint32 _month, uint32 _day, uint32 _hour, uint32 _minute, uint32 _second)
        : year(_year)
        , month(_month)
        , day(_day)
        , hour(_hour)
        , minute(_minute)
        , second(_second)
    {
        JPT_ASSERT(IsValid());
    }

    DateTime::DateTime(const tm& timeData)
    {
        ReadTM(timeData);
    }

    DateTime::DateTime(RawTimeType rawTime)
    {
        ReadRawTime(rawTime);
    }

    DateTime::DateTime(const std::chrono::time_point<std::chrono::system_clock>& clockPoint)
    {
        ReadClockPoint(clockPoint);
    }

    void DateTime::ReadTM(const tm& timeData)
    {
        year   = timeData.tm_year + 1900;
        month  = timeData.tm_mon + 1;
        day    = timeData.tm_mday;
        hour   = timeData.tm_hour;
        minute = timeData.tm_min;
        second = timeData.tm_sec;

        JPT_ASSERT(IsValid());
    }

    void DateTime::ReadRawTime(RawTimeType rawTime)
    {
        tm timeData;
        localtime_s(&timeData, &rawTime);
        ReadTM(timeData);
    }

    void DateTime::ReadClockPoint(const std::chrono::time_point<std::chrono::system_clock>& clockPoint)
    {
        RawTimeType rawTime = std::chrono::system_clock::to_time_t(clockPoint);
        ReadRawTime(rawTime);
    }

    tm DateTime::ToTm() const
    {
        tm timeData;

        timeData.tm_year = year - 1900;
        timeData.tm_mon = month - 1;
        timeData.tm_mday = day;
        timeData.tm_hour = hour;
        timeData.tm_min = minute;
        timeData.tm_sec = second;

        return timeData;
    }

    RawTimeType DateTime::ToRawTime() const
    {
        tm timeData = ToTm();
        return mktime(&timeData);
    }

    std::chrono::time_point<std::chrono::system_clock> DateTime::ToClockPoint() const
    {
        return std::chrono::system_clock::from_time_t(ToRawTime());
    }

    void DateTime::Forward(uint32 seconds)
    {
        RawTimeType rawTime = ToRawTime();
        rawTime += seconds;
        ReadRawTime(rawTime);
    }

    void DateTime::Backward(uint32 seconds)
    {
        RawTimeType rawTime = ToRawTime();
        rawTime -= seconds;
        ReadRawTime(rawTime);
    }

    bool DateTime::operator==(const DateTime& other) const
    {
        return year   == other.year   &&
               month  == other.month  &&
               day    == other.day    &&
               hour   == other.hour   &&
               minute == other.minute &&
               second == other.second;
    }

    DateTime DateTime::operator-(const DateTime& other) const
    {
        DateTime result;

        result.year = year - other.year;

        if (month < other.month)
        {
            --result.year;
            result.month = 12 - (other.month - month);
        }
        else
        {
            result.month = month - other.month;
        }

        if (day < other.day)
        {
            --result.month;
            result.day = 31 - (other.day - day);
        }
        else
        {
            result.day = day - other.day;
        }

        if (hour < other.hour)
        {
            --result.day;
            result.hour = 24 - (other.hour - hour);
        }
        else
        {
            result.hour = hour - other.hour;
        }

        if (minute < other.minute)
        {
            --result.hour;
            result.minute = 60 - (other.minute - minute);
        }
        else
        {
            result.minute = minute - other.minute;
        }

        if (second < other.second)
        {
            --result.minute;
            result.second = 60 - (other.second - second);
        }
        else
        {
            result.second = second - other.second;
        }

        JPT_ASSERT(result.IsValid());
        return result;
    }

    DateTime DateTime::operator+(const DateTime& other) const
    {
        DateTime result;

        result.year = year + other.year;

        if (month + other.month > 12)
        {
            ++result.year;
            result.month = (month + other.month) - 12;
        }
        else
        {
            result.month = month + other.month;
        }

        if (day + other.day > 31)
        {
            ++result.month;
            result.day = (day + other.day) - 31;
        }
        else
        {
            result.day = day + other.day;
        }

        if (hour + other.hour > 23)
        {
            ++result.day;
            result.hour = (hour + other.hour) - 24;
        }
        else
        {
            result.hour = hour + other.hour;
        }

        if (minute + other.minute > 59)
        {
            ++result.hour;
            result.minute = (minute + other.minute) - 60;
        }
        else
        {
            result.minute = minute + other.minute;
        }

        if (second + other.second > 59)
        {
            ++result.minute;
            result.second = (second + other.second) - 60;
        }
        else
        {
            result.second = second + other.second;
        }

        JPT_ASSERT(result.IsValid());
        return result;
    }

    bool DateTime::IsValid() const
    {
        const bool validYear   = year   >= 0 && year   <= 4095;
        const bool validMonth  = month  >= 0 && month  <= 12;
        const bool validDay    = day    >= 0 && day    <= 31;
        const bool validHour   = hour   >= 0 && hour   <= 23;
        const bool validMinute = minute >= 0 && minute <= 59;
        const bool validSecond = second >= 0 && second <= 59;

        return validYear && validMonth && validDay &&
            validHour && validMinute && validSecond;
    }
}