// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Ray3;

import jpt.Concepts;
import jpt.Constants;
import jpt.Vector3;
import jpt.TypeDefs;

namespace jpt
{
    export template<Numeric T>
    struct TRay3
    {
    public:
        Vector3<T> origin;
        Vector3<T> direction;

    public:
        constexpr TRay3() noexcept = default;
        constexpr TRay3(const Vector3<T>& origin, const Vector3<T>& direction) noexcept;

        constexpr Vector3<T> GetPoint(T t) const noexcept;
        constexpr T Distance(const Vector3<T>& point) const noexcept;
        constexpr Vector3<T> ClosestPoint(const Vector3<T>& point) const noexcept;
    };

    template<Numeric T>
    constexpr TRay3<T>::TRay3(const Vector3<T>& origin, const Vector3<T>& direction) noexcept
        : origin(origin)
        , direction(direction)
    {
    }

    template<Numeric T>
    constexpr Vector3<T> TRay3<T>::GetPoint(T t) const noexcept
    {
        return origin + direction * t;
    }

    template<Numeric T>
    constexpr T TRay3<T>::Distance(const Vector3<T>& point) const noexcept
    {
        const T dot = Vector3<T>::Dot(point - origin, direction);
        T distance = kInvalidValue<T>;

        // If the point is behind the ray's origin, return the distance to the origin
        if (dot < 0)
        {
            distance = Vector3<T>::Distance(point, origin);
        }
        else
        {
            // Otherwise, return the distance to the point on the ray closest to the point
            const Vector3<T> projection = origin + direction * (dot / direction.Length2());
            distance = Vector3<T>::Distance(point, projection);
        }

        return distance;
    }

    template<Numeric T>
    constexpr Vector3<T> TRay3<T>::ClosestPoint(const Vector3<T>& point) const noexcept
    {
        const Vector3<T> op = point - origin;
        const T t = Vector3<T>::Dot(op, direction);
        if (t <= 0)
        {
            return origin;
        }
        else
        {
            const T d = direction.Length2();
            if (t >= d)
            {
                return origin + direction;
            }
            else
            {
                return origin + direction * (t / d);
            }
        }
    }
}

export using Ray3 = jpt::TRay3<Precision>;