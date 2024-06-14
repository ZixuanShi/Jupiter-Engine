// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Triangle3;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Vector3;
import jpt.Math;

namespace jpt
{
	export template<Numeric T>
	struct Triangle3
	{
	public:
		Vector3<T> a = static_cast<Vector3<T>>(0);
		Vector3<T> b = static_cast<Vector3<T>>(0);
		Vector3<T> c = static_cast<Vector3<T>>(0);

	public:
		constexpr Triangle3() = default;
		constexpr Triangle3(Vector3<T> _a, Vector3<T> _b, Vector3<T> _c);

		constexpr T Perimeter() const;
		constexpr T Area() const;
		constexpr Vector3<T> Normal() const;
		constexpr T Distance(Vector3<T> point) const;
	};

	template<Numeric T>
	constexpr Triangle3<T>::Triangle3(Vector3<T> _a, Vector3<T> _b, Vector3<T> _c)
		: a(_a), b(_b), c(_c)
	{
	}

	template<Numeric T>
	constexpr T Triangle3<T>::Perimeter() const
	{
		return Vector3<T>::Distance(a, b) + Vector3<T>::Distance(b, c) + Vector3<T>::Distance(c, a);
	}

	template<Numeric T>
	constexpr T Triangle3<T>::Area() const
	{
		const T s = Perimeter() * static_cast<T>(0.5);
		return std::sqrt(s * (s - Vector3<T>::Distance(a, b)) * (s - Vector3<T>::Distance(b, c)) * (s - Vector3<T>::Distance(c, a)));
	}

	template<Numeric T>
	constexpr Vector3<T> Triangle3<T>::Normal() const
	{
		const Vector3<T> edge1 = b - a;
		const Vector3<T> edge2 = c - a;
		Vector3<T> normal = edge1.Cross(edge2);
		normal.Normalize();
		return normal;
	}

	template<Numeric T>
	constexpr T Triangle3<T>::Distance(Vector3<T> point) const
	{
		// Compute the edges of the triangle
		const Vec3f edgeBa = b - a;
		const Vec3f edgeCa = c - a;

		// Compute the normal of the triangle
		const Vec3f cross = edgeBa.Cross(edgeCa);
		const Vec3f normal = cross.Normalized();

		// Compute the vector from the point to the triangle
		const Vec3f pointToTriangle = a - point;
		const float distance = Abs(pointToTriangle.Dot(normal));

		return distance;
	}
}

export using Triangle3f = jpt::Triangle3<float>;