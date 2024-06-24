// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

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

		constexpr Vector3<T> GetLine() const;
		constexpr T Distance(const Vector3<T>& point) const;
	};

	template<Numeric T>
	constexpr Line3<T>::Line3(const Vector3<T>& a, const Vector3<T>& b)
		: a(a)
		, b(b)
	{
	}

	template<Numeric T>
	constexpr Vector3<T> Line3<T>::GetLine() const
	{
		const T x = a.y - b.y;
		const T y = b.x - a.x;
		const T z = a.x * b.y - a.y * b.x;
		return Vector3<T>(x, y, z);
	}

	template<Numeric T>
	constexpr T Line3<T>::Distance(const Vector3<T>& point) const
	{
		const Vector3<T> lineData = GetLine();
		const T distance = Abs(lineData.x * point.x + lineData.y * point.y + lineData.z) / std::sqrt(lineData.x * lineData.x + lineData.y * lineData.y);
		return distance;
	}
}

export using Line3f = jpt::Line3<float>;
export using Line3d = jpt::Line3<double>;
