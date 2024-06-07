// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Matrix33;

import jpt.Concepts;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.Math;
import jpt.Vector2;
import jpt.Vector3;

namespace jpt
{
	export template<Numeric T>
	struct Matrix33
	{
	public:
		T m[3][3];

	public:
		constexpr Matrix33();
	};

	template<Numeric T>
	constexpr Matrix33<T>::Matrix33()
		: m{ { 1, 0, 0 },
			 { 0, 1, 0 },
			 { 0, 0, 1 } }
	{
	}
}
