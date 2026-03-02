// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

export module jpt.Plane;

import jpt.Concepts;
import jpt.Vector3;
import jpt.TypeDefs;
import jpt.Math;
import jpt.Constants;

export namespace jpt
{
    /** Represents a plane in 3D space using the equation: ax + by + cz + d = 0
        Where (a, b, c) is the normal vector and d is the distance from origin
    
        Alternatively: normal � point + distance = 0 */
    template<Floating T>
    struct TPlane
    {
    public:
        Vector3<T> m_normal = Vector3<T>(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0)); /**< Normalized normal vector of the plane. */
        T m_distance = static_cast<T>(0);        /**< Distance from the origin to the plane along the normal. */

    public:
        constexpr TPlane() = default;
        constexpr TPlane(const Vector3<T>& normal, T distance);
        constexpr TPlane(const Vector3<T>& normal, const Vector3<T>& point);
        constexpr TPlane(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c);

    public:
        [[nodiscard]] constexpr T DistanceSigned(const Vector3<T>& point) const noexcept;
        [[nodiscard]] constexpr T Distance(const Vector3<T>& point) const noexcept;
        [[nodiscard]] constexpr Vector3<T> ProjectPoint(const Vector3<T>& point) const noexcept;
        [[nodiscard]] constexpr Vector3<T> ProjectVec3(const Vector3<T>& vec3) const noexcept;
    };

    template<Floating T>
    constexpr TPlane<T>::TPlane(const Vector3<T>& normal, T distance)
        : m_normal(normal)
        , m_distance(distance)
    {
        JPT_ASSERT(AreValuesClose(normal.Length2(), static_cast<T>(1)), "Plane normal must be normalized");
    }

    template<Floating T>
    constexpr TPlane<T>::TPlane(const Vector3<T>& normal, const Vector3<T>& point)
        : m_normal(normal)
        , m_distance(-normal.Dot(point))
    {
        JPT_ASSERT(AreValuesClose(normal.Length2(), static_cast<T>(1)), "Plane normal must be normalized");
    }

    template<Floating T>
    constexpr TPlane<T>::TPlane(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c)
    {
        const Vector3<T> ab = b - a;
        const Vector3<T> ac = c - a;
        m_normal = ab.Cross(ac).Normalized();
        m_distance = -m_normal.Dot(a);
    }

    template<Floating T>
    constexpr T TPlane<T>::DistanceSigned(const Vector3<T>& point) const noexcept
    {
        // Formula: distance = normal . point + distance
        return m_normal.Dot(point) + m_distance;
    }

    template<Floating T>
    constexpr T TPlane<T>::Distance(const Vector3<T>& point) const noexcept
    {
        return Abs(DistanceSigned(point));
    }

    template<Floating T>
    constexpr Vector3<T> TPlane<T>::ProjectPoint(const Vector3<T>& point) const noexcept
    {
        // Formula: projectedPoint = point - distance * normal
        const T distance = DistanceSigned(point);
        return point - (distance * m_normal);
    }

    template<Floating T>
    constexpr Vector3<T> TPlane<T>::ProjectVec3(const Vector3<T>& vec3) const noexcept
    {
        // Formula: projectedVec3 = vec3 - (vec3 . normal) * normal
        const T dot = vec3.Dot(m_normal);
        return vec3 - (dot * m_normal);
    }
}

export using Plane  = jpt::TPlane<Precision>;
export using Planef = jpt::TPlane<float32>;
export using Planed = jpt::TPlane<float64>;