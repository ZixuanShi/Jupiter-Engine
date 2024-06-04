// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Line2;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;

namespace jpt
{
	/** infinite length across two points */
	export template<Numeric T>
	struct Line2
	{
	public:
		Vector2<T> a;
		Vector2<T> b;

	public:
		constexpr Line2() = default;
		constexpr Line2(Vector2<T> a, Vector2<T> b);

		constexpr Vector3<T> GetLine() const;
		constexpr T Distance(Vector2<T> point) const;
	};

	template<Numeric T>
	constexpr Line2<T>::Line2(Vector2<T> a, Vector2<T> b)
		: a(a)
		, b(b) 
	{
	}

	template<Numeric T>
	constexpr Vector3<T> Line2<T>::GetLine() const
	{
		const T x = a.y - b.y;
		const T y = b.x - a.x;
		const T z = a.x * b.y - a.y * b.x;
		return Vector3<T>(x, y, z);
	}

	template<Numeric T>
	constexpr T Line2<T>::Distance(Vector2<T> point) const
	{
		const Vector3<T> lineData = GetLine();
		const T distance = std::abs(lineData.x * point.x + lineData.y * point.y + lineData.z) / std::sqrt(lineData.x * lineData.x + lineData.y * lineData.y);
		return distance;
	}
}

export using Line2i = jpt::Line2<int32>;
export using Line2f = jpt::Line2<float32>;
export using Line2d = jpt::Line2<float64>;

template<> constexpr bool jpt::IsTrivial<Line2i> = true;
template<> constexpr bool jpt::IsTrivial<Line2f> = true;