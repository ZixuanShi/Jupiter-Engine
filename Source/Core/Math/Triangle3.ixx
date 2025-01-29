// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Triangle3;

import jpt.Concepts;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Vector3;
import jpt.Math;

namespace jpt
{
	export template<Numeric T>
	struct TTriangle3
	{
	public:
		TVector3<T> a = static_cast<TVector3<T>>(0);
		TVector3<T> b = static_cast<TVector3<T>>(0);
		TVector3<T> c = static_cast<TVector3<T>>(0);

	public:
		constexpr TTriangle3() = default;
		constexpr TTriangle3(const TVector3<T>& _a, const TVector3<T>& _b, const TVector3<T>& _c);

		constexpr T Perimeter() const;
		constexpr T Area() const;
		constexpr TVector3<T> Normal() const;
		constexpr T Distance(const TVector3<T>& point) const;
		constexpr bool Inside(const TVector3<T>& point) const;
	};

	template<Numeric T>
	constexpr TTriangle3<T>::TTriangle3(const TVector3<T>& _a, const TVector3<T>& _b, const TVector3<T>& _c)
		: a(_a), b(_b), c(_c)
	{
	}

	template<Numeric T>
	constexpr T TTriangle3<T>::Perimeter() const
	{
		return TVector3<T>::Distance(a, b) + TVector3<T>::Distance(b, c) + TVector3<T>::Distance(c, a);
	}

	template<Numeric T>
	constexpr T TTriangle3<T>::Area() const
	{
		const T s = Perimeter() * static_cast<T>(0.5);
		return std::sqrt(s * (s - TVector3<T>::Distance(a, b)) * (s - TVector3<T>::Distance(b, c)) * (s - TVector3<T>::Distance(c, a)));
	}

	template<Numeric T>
	constexpr TVector3<T> TTriangle3<T>::Normal() const
	{
		// Compute the edges of the triangle
		const TVector3 edge1 = b - a;
		const TVector3 edge2 = c - a;

		// Compute the normal of the triangle
		TVector3 normal = edge1.Cross(edge2);
		normal.Normalize();
		return normal;
	}

	template<Numeric T>
	constexpr T TTriangle3<T>::Distance(const TVector3<T>& point) const
	{
		const TVector3 normal = Normal();
		const TVector3 aToP = point - a;
		const T distance = Abs(aToP.Dot(normal));

		return distance;
	}

	template<Numeric T>
	constexpr bool TTriangle3<T>::Inside(const TVector3<T>& point) const
	{
		// https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates

		const TVector3 BA = b - a;
		const TVector3 CA = c - a;
		const TVector3 PA = point - a;

		const T dot00 = BA.Dot(BA);
		const T dot01 = BA.Dot(CA);
		const T dot02 = BA.Dot(PA);
		const T dot11 = CA.Dot(CA);
		const T dot12 = CA.Dot(PA);

		const T invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		const T u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		const T v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		return (u >= 0) && 
			   (v >= 0) && 
			   (u + v <= static_cast<T>(1) + Constants<T>::kEpsilon);
	}
}

export using Triangle3  = jpt::TTriangle3<Precision>;
export using Triangle3f = jpt::TTriangle3<float32>;