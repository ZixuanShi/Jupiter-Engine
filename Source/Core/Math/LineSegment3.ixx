// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.LineSegment3;

import jpt.TypeDefs;
import jpt.Concepts;
import jpt.Vector3;

namespace jpt
{
	export template<Numeric T>
	struct TLineSegment3
	{
	public:
		Vector3<T> a;
		Vector3<T> b;

	public:
		constexpr T Length() const noexcept;

		constexpr T Distance(const Vector3<T>& point) const noexcept;
		constexpr T Dist(const Vector3<T>& point) const noexcept;

		constexpr Vector3<T> Direction() const noexcept;
		constexpr Vector3<T> Dir() const noexcept;

		constexpr Vector3<T> Project(const Vector3<T>& point) const noexcept;

		constexpr bool Intersects(const TLineSegment3<T>& other) const noexcept;
	};

	template<Numeric T>
	constexpr T TLineSegment3<T>::Length() const noexcept
	{
		return a.Distance(b);
	}

	template<Numeric T>
	constexpr T TLineSegment3<T>::Distance(const Vector3<T>& point) const noexcept
	{
		const Vector3<T> pointOnLine = Project(point);
		const Vector3<T> pointToLine = point - pointOnLine;
		return pointToLine.Length();
	}

	template<Numeric T>
	constexpr T TLineSegment3<T>::Dist(const Vector3<T>& point) const noexcept
	{
		return Distance(point);
	}

	template<Numeric T>
	constexpr Vector3<T> TLineSegment3<T>::Direction() const noexcept
	{
		return (b - a).Normalize();
	}

	template<Numeric T>
	constexpr Vector3<T> TLineSegment3<T>::Dir() const noexcept
	{
		return Direction();
	}

	template<Numeric T>
	constexpr Vector3<T> TLineSegment3<T>::Project(const Vector3<T>& point) const noexcept
	{
		const Vector3<T> ab = b - a;
		const Vector3<T> ap = point - a;
		T t = ab.Dot(ap) / ab.Dot(ab);
		ClampTo(t, static_cast<T>(0), static_cast<T>(1));
		return a + ab * t;
	}

	template<Numeric T>
	constexpr bool TLineSegment3<T>::Intersects(const TLineSegment3<T>& other) const noexcept
	{
		const Vector3<T> p1 = a;
		const Vector3<T> q1 = b;
		const Vector3<T> p2 = other.a;
		const Vector3<T> q2 = other.b;

		const Vector3<T> r = q1 - p1;
		const Vector3<T> s = q2 - p2;

		const T rxs = r.Cross(s);
		const Vector3<T> qp = p2 - p1;
		const T t = qp.Cross(s) / rxs;
		const T u = qp.Cross(r) / rxs;

		return rxs != static_cast<T>(0) && 
			     t >= static_cast<T>(0) && 
			     t <= static_cast<T>(1) && 
			     u >= static_cast<T>(0) && 
			     u <= static_cast<T>(1);
	}
}

export using LineSegment3  = jpt::TLineSegment3<Precision>;
export using LineSegment3f = jpt::TLineSegment3<float32>;
export using LineSegment3d = jpt::TLineSegment3<float64>;