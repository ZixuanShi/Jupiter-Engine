// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"

export module jpt.Function;

import jpt.TypeTraits;
import jpt.Utilities;

export namespace jpt
{
	template<class>
	class Function;

	/** Wrapper for a function. Can be used as Callbacks */
	template<class TReturn, class... TArgs>
	class Function<TReturn(TArgs...)>
	{
	public:
		using FunctionType = TReturn(TArgs...);

	public:
		constexpr Function() = default;
	};
}