// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.LineSegment2;

import jpt.Concepts;
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
		[[nodiscard]] constexpr T Length() const noexcept;
	};

	template<Numeric T>
	constexpr T LineSegment2<T>::Length() const noexcept
	{
		return a.Distance(b);
	}
}

export using LineSegment2f = jpt::LineSegment2<float>;
export using LineSegment2d = jpt::LineSegment2<double>;

template<> constexpr bool jpt::IsTrivial<LineSegment2f> = true;