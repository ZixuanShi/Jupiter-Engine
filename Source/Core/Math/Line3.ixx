// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <cmath>

export module jpt.Line3;

import jpt.TypeDefs;
import jpt.Concepts;
import jpt.TVector3;
import jpt.Math;

namespace jpt
{
	/** infinite length across two points */
	export template<Numeric T>
	struct TLine3
	{
	public:
		TVector3<T> a;
		TVector3<T> b;

	public:
		constexpr TLine3() = default;
		constexpr TLine3(const TVector3<T>& a, const TVector3<T>& b);

		/** Calculate the direction vector of the line */
		constexpr TVector3<T> Direction() const;
		constexpr TVector3<T> Dir() const;

		/** Closest distance from point to line */
		constexpr T Distance(const TVector3<T>& point) const;
		constexpr T Dist(const TVector3<T>& point) const;

		/** The projected point is the closest point on the line to the given point
			We can find it by moving from the original point perpendicular to the line */
		constexpr TVector3<T> Project(const TVector3<T>& point) const;
	};

	template<Numeric T>
	constexpr TLine3<T>::TLine3(const TVector3<T>& a, const TVector3<T>& b)
		: a(a)
		, b(b)
	{
	}

	template<Numeric T>
	constexpr TVector3<T> TLine3<T>::Direction() const
	{
		return (b - a).Normalized();
	}

	template<Numeric T>
	constexpr TVector3<T> TLine3<T>::Dir() const
	{
		return Direction(); 
	}

	template<Numeric T>
	constexpr T TLine3<T>::Distance(const TVector3<T>& point) const
	{
		const TVector3<T> dir = Dir();
		const TVector3<T> aToP = point - a;
		const TVector3<T> cross = aToP.Cross(dir);
		return cross.Length();
	}

	template<Numeric T>
	constexpr T TLine3<T>::Dist(const TVector3<T>& point) const 
	{
		return Distance(point); 
	}

	template<Numeric T>
	constexpr TVector3<T> TLine3<T>::Project(const TVector3<T>& point) const
	{
		const TVector3<T> dir = Dir();
		const TVector3<T> aToP = point - a;

		const T t = aToP.Dot(dir);
		const TVector3<T> projected = a + dir * t;
		return projected;
	}
}

export using Line3  = jpt::TLine3<Precision>;
export using Line3f = jpt::TLine3<float32>;
export using Line3d = jpt::TLine3<float64>;
