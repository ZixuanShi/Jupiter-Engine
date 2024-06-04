// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Ray2;

import jpt.Concepts;
import jpt.Vector2;

namespace jpt
{
	export template<Numeric T>
	struct Ray2
	{
	public:
		Vector2<T> origin;
		Vector2<T> direction;
	};
}
