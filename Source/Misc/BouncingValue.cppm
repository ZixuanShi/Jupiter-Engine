// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

export module jpt.BouncingValue;

import jpt.TypeDefs;

export namespace jpt
{
    template<typename T>
    struct BouncingValue
    {
    public:
        T min = static_cast<T>(0);
        T max = static_cast<T>(1);
        T current = static_cast<T>(0);
        T speed = static_cast<T>(1);
        bool increasing = true;

    public:
        constexpr BouncingValue() = default;
        constexpr BouncingValue(T _min, T _max);
        constexpr BouncingValue(T _min, T _max, T _current);
        constexpr BouncingValue(T _min, T _max, T _current, T _speed);

        constexpr void Update(TimePrecision deltaSeconds);

        constexpr operator T() const { return current; }
    };

    template<typename T>
    constexpr BouncingValue<T>::BouncingValue(T _min, T _max)
        : min(_min)
        , max(_max)
        , current(_min)
    {
        JPT_ASSERT(_min < _max);
        JPT_ASSERT(_min <= current && current <= _max);
    }

    template<typename T>
    constexpr BouncingValue<T>::BouncingValue(T _min, T _max, T _current)
        : min(_min)
        , max(_max)
        , current(_current)
    {
        JPT_ASSERT(_min < _max);
        JPT_ASSERT(_min <= current && current <= _max);
    }

    template<typename T>
    constexpr BouncingValue<T>::BouncingValue(T _min, T _max, T _current, T _speed)
        : min(_min)
        , max(_max)
        , current(_current)
        , speed(_speed)
    {
        JPT_ASSERT(_min < _max);
        JPT_ASSERT(_min <= current && current <= _max);
    }

    template<typename T>
    constexpr void BouncingValue<T>::Update(TimePrecision deltaSeconds)
    {
        if (increasing)
        {
            current += speed * static_cast<T>(deltaSeconds);
        }
        else
        {
            current -= speed * static_cast<T>(deltaSeconds);
        }

        if (current >= max)
        {
            increasing = false;
        }
        else if (current <= min)
        {
            increasing = true;
        }
    }
}