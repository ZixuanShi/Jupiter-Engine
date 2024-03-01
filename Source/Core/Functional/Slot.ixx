// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Slot;

export import jpt.Function;

import jpt.DynamicArray;
import jpt.Utilities;
import jpt.Math;
import jpt.TypeTraits;

export namespace jpt
{
	template<class>
	class Slot;

	/** Array of Functions as Slot */
	template<class TReturn, class... TArgs>
	class Slot<TReturn(TArgs...)>
	{
		using TFunc = Function<TReturn(TArgs...)>;

	private:

	public:
	};
}