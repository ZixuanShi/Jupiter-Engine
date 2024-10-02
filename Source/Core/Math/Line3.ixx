// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

#include <cmath>

export module jpt.Line3;

import jpt.Concepts;
import jpt.Vector3;
import jpt.Math;

namespace jpt
{
	/** infinite length across two points */
	export template<Numeric T>
		struct Line3
	{
	public:
		Vector3<T> a;
		Vector3<T> b;

	public:
		constexpr Line3() = default;
		constexpr Line3(const Vector3<T>& a, const Vector3<T>& b);

		/** Calculate the direction vector of the line */
		constexpr Vector3<T> Direction() const;
		constexpr Vector3<T> Dir() const;

		/** Closest distance from point to line */
		constexpr T Distance(const Vector3<T>& point) const;
		constexpr T Dist(const Vector3<T>& point) const;

		/** The projected point is the closest point on the line to the given point
			We can find it by moving from the original point perpendicular to the line */
		constexpr Vector3<T> Project(const Vector3<T>& point) const;
	};

	template<Numeric T>
	constexpr Line3<T>::Line3(const Vector3<T>& a, const Vector3<T>& b)
		: a(a)
		, b(b)
	{
	}

	template<Numeric T>
	constexpr Vector3<T> Line3<T>::Direction() const
	{
		return (b - a).Normalized();
	}

	template<Numeric T>
	constexpr Vector3<T> Line3<T>::Dir() const
	{
		return Direction(); 
	}

	template<Numeric T>
	constexpr T Line3<T>::Distance(const Vector3<T>& point) const
	{
		const Vector3<T> ab = b - a;
		const Vector3<T> ap = point - a;
		const Vector3<T> cross = ab.Cross(ap);
		return Sqrt(cross.Length2() / ab.Length2());
	}

	template<Numeric T>
	constexpr T Line3<T>::Dist(const Vector3<T>& point) const 
	{
		return Distance(point); 
	}

	template<Numeric T>
	constexpr Vector3<T> Line3<T>::Project(const Vector3<T>& point) const
	{
		const Vector3<T> dir = Dir();
		const Vector3<T> pToLine = point - a;

		const T t = pToLine.Dot(dir);
		const Vector3<T> projected = a + dir * t;
		return projected;
	}
}

export using Line3f = jpt::Line3<float>;
export using Line3d = jpt::Line3<double>;
