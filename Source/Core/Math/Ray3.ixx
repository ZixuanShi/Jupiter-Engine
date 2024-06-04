// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Ray3;

import jpt.Concepts;
import jpt.Vector3;

namespace jpt
{
	export template<Numeric T>
	struct Ray3
	{
	public:
		Vector3<T> origin;
		Vector3<T> direction;
	};
}
