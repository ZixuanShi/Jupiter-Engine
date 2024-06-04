// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.LineSegment3;

import jpt.Concepts;
import jpt.Vector3;

namespace jpt
{
	export template<Numeric T>
	struct LineSegment3
	{
	public:
		Vector3<T> a;
		Vector3<T> b;

	public:
		[[nodiscard]] constexpr T Length() const noexcept;
	};

	template<Numeric T>
	constexpr T LineSegment3<T>::Length() const noexcept
	{
		return a.Distance(b);
	}
}

export using LineSegment3f = jpt::LineSegment3<float>;
export using LineSegment3d = jpt::LineSegment3<double>;