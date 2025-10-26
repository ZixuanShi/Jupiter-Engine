// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Box2;

import jpt.Concepts;
import jpt.Vector2;
import jpt.Math;
import jpt.TypeDefs;

export namespace jpt
{
    template<Numeric T>
    struct TBox2
    {
    public:
        Vector2<T> min;
        Vector2<T> max;

    public:
        constexpr TBox2() noexcept = default;
        constexpr TBox2(const Vector2<T>& min, const Vector2<T>& max) noexcept;

        constexpr Vector2<T> Center() const noexcept;
        constexpr Vector2<T> Size() const noexcept;
        constexpr Vector2<T> HalfSize() const noexcept;

        constexpr bool Inside(const Vector2<T>& point) const noexcept;
        constexpr bool Intersects(const TBox2<T>& other) const noexcept;
    };

    template<Numeric T>
    constexpr bool operator==(const TBox2<T>& lhs, const TBox2<T>& rhs) noexcept
    {
        return lhs.min == rhs.min && lhs.max == rhs.max;
    }

    template<Numeric T>
    constexpr TBox2<T>::TBox2(const Vector2<T>& min, const Vector2<T>& max) noexcept
        : min(min)
        , max(max)
    {
    }

    template<Numeric T>
    constexpr Vector2<T> TBox2<T>::Center() const noexcept
    {
        return (min + max) / static_cast<T>(2);
    }

    template<Numeric T>
    constexpr Vector2<T> TBox2<T>::Size() const noexcept
    {
        return max - min;
    }

    template<Numeric T>
    constexpr Vector2<T> TBox2<T>::HalfSize() const noexcept
    {
        return Size() / 2;
    }

    template<Numeric T>
    constexpr bool TBox2<T>::Inside(const Vector2<T>& point) const noexcept
    {
        return (point.x >= min.x && point.x <= max.x) && 
               (point.y >= min.y && point.y <= max.y);
    }

    template<Numeric T>
    constexpr bool TBox2<T>::Intersects(const TBox2<T>& other) const noexcept
    {
        if (min.x > other.max.x)
        {
            return false;
        }
        if (max.x < other.min.x)
        {
            return false;
        }
        if (min.y > other.max.y)
        {
            return false;
        }
        if (max.y < other.min.y)
        {
            return false;
        }

        return true;
    }
}

export using Box2 = jpt::TBox2<Precision>;
export using Box2f = jpt::TBox2<float32>;
export using Box2i = jpt::TBox2<int32>;