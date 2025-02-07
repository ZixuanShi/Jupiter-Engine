// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Triangle2;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Vector2;
import jpt.Math;

namespace jpt
{
	export template<Numeric T>
	struct TTriangle2
	{
	public:
		Vector2<T> a = static_cast<Vector2<T>>(0);
		Vector2<T> b = static_cast<Vector2<T>>(0);
		Vector2<T> c = static_cast<Vector2<T>>(0);

	public:
		constexpr TTriangle2() = default;
		constexpr TTriangle2(Vector2<T> _a, Vector2<T> _b, Vector2<T> _c);

		constexpr T Perimeter() const;
		constexpr T Area() const;
		constexpr bool Inside(Vector2<T> point) const;
	};

	template<Numeric T>
	constexpr TTriangle2<T>::TTriangle2(Vector2<T> _a, Vector2<T> _b, Vector2<T> _c)
		: a(_a), b(_b), c(_c)
	{
	}

	template<Numeric T>
	constexpr T TTriangle2<T>::Perimeter() const
	{
		return Vector2<T>::Distance(a, b) + Vector2<T>::Distance(b, c) + Vector2<T>::Distance(c, a);
	}

	template<Numeric T>
	constexpr T TTriangle2<T>::Area() const
	{
		const T s = Perimeter() * static_cast<T>(0.5);
		return std::sqrt(s * (s - Vector2<T>::Distance(a, b)) * (s - Vector2<T>::Distance(b, c)) * (s - Vector2<T>::Distance(c, a)));
	}

	template<Numeric T>
	constexpr bool TTriangle2<T>::Inside(Vector2<T> point) const
	{
		// Barycentric coordinates

		const float u = (b.y - c.y) * (point.x - c.x) + (c.x - b.x) * (point.y - c.y);
		const float v = (c.y - a.y) * (point.x - c.x) + (a.x - c.x) * (point.y - c.y);
		const float w = 1.0f - u - v;

		return u >= 0.0f && v >= 0.0f && w >= 0.0f;
	}
}

export using Triangle2 = jpt::TTriangle2<Precision>;
export using Triangle2f = jpt::TTriangle2<float32>;