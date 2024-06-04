// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Line3;

import jpt.Concepts;
import jpt.Vector3;

namespace jpt
{
	/** infinite length across two points */
	export template<Numeric T>
		struct Line3
	{
	public:
		Vector3<T> a;
		Vector3<T> b;
	};
}

export using Line3f = jpt::Line3<float>;
export using Line3d = jpt::Line3<double>;
