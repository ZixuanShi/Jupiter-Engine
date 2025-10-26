// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.LineSegment2;

import jpt.Concepts;
import jpt.Math;
import jpt.Vector2;

namespace jpt
{
    /** Has two definite endpoints and a length */
    export template<Numeric T>
    struct TLineSegment2
    {
    public:
        Vector2<T> a;
        Vector2<T> b;

    public:
        constexpr Vector2<T> Direction() const noexcept;
        constexpr Vector2<T> Dir() const noexcept;

        constexpr T Length() const noexcept;

        /** @return Closest distance from line segment to point in 2d space */
        constexpr T Distance(const Vector2<T>& point) const noexcept;
        constexpr T Dist(const Vector2<T>& point) const noexcept;

        /** @return Projected point to this line segment */
        constexpr Vector2<T> Project(const Vector2<T>& point) const noexcept;
    };

    template<Numeric T>
    constexpr Vector2<T> TLineSegment2<T>::Direction() const noexcept
    {
        return (b - a).Normalized();
    }

    template<Numeric T>
    constexpr Vector2<T> TLineSegment2<T>::Dir() const noexcept
    {
        return Direction(); 
    }

    template<Numeric T>
    constexpr T TLineSegment2<T>::Length() const noexcept
    {
        return a.Distance(b);
    }

    template<Numeric T>
    constexpr T TLineSegment2<T>::Distance(const Vector2<T>& point) const noexcept
    {
        const Vector2<T> pointOnLine = Project(point);
        const Vector2<T> pointToLine = point - pointOnLine;
        return pointToLine.Length();
    }

    template<Numeric T>
    constexpr T TLineSegment2<T>::Dist(const Vector2<T>& point) const noexcept
    { 
        return Distance(point); 
    }

    template<Numeric T>
    constexpr Vector2<T> TLineSegment2<T>::Project(const Vector2<T>& point) const noexcept
    {
        const Vector2<T> ab = b - a;
        const Vector2<T> ap = point - a;
        T t = ab.Dot(ap) / ab.Dot(ab);
        ClampTo(t, static_cast<T>(0), static_cast<T>(1));
        return a + ab * t;
    }
}

export using LineSegment2  = jpt::TLineSegment2<Precision>;
export using LineSegment2f = jpt::TLineSegment2<float32>;
export using LineSegment2d = jpt::TLineSegment2<float64>;

template<> constexpr bool jpt::IsTrivial<LineSegment2f> = true;