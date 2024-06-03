// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Edge2;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Vector2;
import jpt.Math;

namespace jpt
{
	export template<Numeric T>
	struct Edge2
	{
	public:
		Vector2<T> begin;
		Vector2<T> end;

	public:
		constexpr Edge2() = default;
	};
}