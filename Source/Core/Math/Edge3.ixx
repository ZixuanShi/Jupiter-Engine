// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>

export module jpt.Edge3;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Vector3;
import jpt.Math;

namespace jpt
{
	export template<Numeric T>
	struct Edge3
	{
	public:
		Vector3<T> begin;
		Vector3<T> end;

	public:
		constexpr Edge3() = default;
	};
}