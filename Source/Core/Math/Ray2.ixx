// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Ray2;

import jpt.Concepts;
import jpt.Vector2;
import jpt.TypeDefs;

namespace jpt
{
	/** Has one endpoint, endless towards one direction */
	export template<Numeric T>
	struct Ray2
	{
	public:
		Vector2<T> origin;
		Vector2<T> direction;

	public:

	};
}

export using Ray2i = jpt::Ray2<int32>;
export using Ray2f = jpt::Ray2<float32>;
export using Ray2d = jpt::Ray2<float64>;

template<> constexpr bool jpt::IsTrivial<Ray2f> = true;
template<> constexpr bool jpt::IsTrivial<Ray2i> = true;