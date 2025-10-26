// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <cmath>

export module jpt.Line2;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.Math;
import jpt.Vector2;
import jpt.Vector3;

namespace jpt
{
    /** infinite length across two points */
    export template<Numeric T>
    struct TLine2
    {
    public:
        Vector2<T> a;
        Vector2<T> b;

    public:
        constexpr TLine2() = default;
        constexpr TLine2(const Vector2<T>& a, const Vector2<T>& b);

        /** ax + by + c = 0 */
        constexpr Vector3<T> Coefficients() const;

        /** distance from point to line */
        constexpr T Distance(const Vector2<T>& point) const;
        constexpr T Dist(const Vector2<T>& point) const;

        /** The projected point is the closest point on the line to the given point
            We can find it by moving from the original point perpendicular to the line */
        constexpr Vector2<T> Project(const Vector2<T>& point) const;
    };

    template<Numeric T>
    constexpr TLine2<T>::TLine2(const Vector2<T>& a, const Vector2<T>& b)
        : a(a)
        , b(b) 
    {
    }

    template<Numeric T>
    constexpr Vector3<T> TLine2<T>::Coefficients() const
    {
        const T x = a.y - b.y;
        const T y = b.x - a.x;
        const T z = a.x * b.y - a.y * b.x;
        return Vector3<T>(x, y, z);
    }

    template<Numeric T>
    constexpr T TLine2<T>::Distance(const Vector2<T>& point) const
    {
        // d = |Ax0 + By0 + C| / sqrt(A^2 + B^2)

        const Vector3<T> coefficients = Coefficients();
        
        const T numerator = Abs(coefficients.x * point.x + coefficients.y * point.y + coefficients.z);
        const T denominator = Sqrt(coefficients.x * coefficients.x + coefficients.y * coefficients.y);
        
        JPT_ASSERT(denominator != 0.0f);
        return numerator / denominator;
    }

    template<Numeric T>
    constexpr T TLine2<T>::Dist(const Vector2<T>& point) const
    {
        return Distance(point);
    }

    template<Numeric T>
    constexpr Vector2<T> TLine2<T>::Project(const Vector2<T>& point) const
    {
        const Vector3<T> coefficients = Coefficients();

        const T denominator = coefficients.x * coefficients.x + coefficients.y * coefficients.y;
        JPT_ASSERT(denominator != 0.0f);

        const T invDenominator = T(1) / denominator;
        const T t = -(coefficients.x * point.x + coefficients.y * point.y + coefficients.z) * invDenominator;
        return Vector2<T>(point.x + t * coefficients.x, point.y + t * coefficients.y);
    }
}

export using Line2  = jpt::TLine2<Precision>;
export using Line2i = jpt::TLine2<int32>;
export using Line2f = jpt::TLine2<float32>;
export using Line2d = jpt::TLine2<float64>;

template<> constexpr bool jpt::IsTrivial<Line2i> = true;
template<> constexpr bool jpt::IsTrivial<Line2f> = true;