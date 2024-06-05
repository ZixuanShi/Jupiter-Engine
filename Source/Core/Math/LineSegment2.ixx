// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.LineSegment2;

import jpt.Concepts;
import jpt.Math;
import jpt.Vector2;

namespace jpt
{
	/** Has two definite endpoints and a length */
	export template<Numeric T>
	struct LineSegment2
	{
	public:
		Vector2<T> a;
		Vector2<T> b;

	public:
		constexpr Vector2<T> Direction() const noexcept;
		constexpr T Length() const noexcept;
		constexpr T Distance(Vector2<T> point) const noexcept;
	};

	template<Numeric T>
	constexpr Vector2<T> LineSegment2<T>::Direction() const noexcept
	{ 
		return b - a; 
	}

	template<Numeric T>
	constexpr T LineSegment2<T>::Length() const noexcept
	{
		return a.Distance(b);
	}

	template<Numeric T>
	constexpr T LineSegment2<T>::Distance(Vector2<T> point) const noexcept
	{
		const Vec2f direction = Direction();
		const float length2 = direction.Length2();

		// If the line segment is a point, return the distance between the point and the segment's endpoint
		if (AreValuesClose(length2, 0.0f))
		{
			return (point - a).Length();
		}

		// Calculate the parameter t of the closest point on the line segment
		const Vec2f toPoint = point - a;
		float t = Vec2f::Dot(toPoint, direction) / length2;
		ClampTo(t, 0.0f, 1.0f);

		// Calculate the closest point on the line segment
		const Vec2f projection = a + direction * t;

		// Return the distance between the point and the closest point on the line segment
		const float distance = (point - projection).Length();
		return distance;
	}
}

export using LineSegment2f = jpt::LineSegment2<float>;
export using LineSegment2d = jpt::LineSegment2<double>;

template<> constexpr bool jpt::IsTrivial<LineSegment2f> = true;