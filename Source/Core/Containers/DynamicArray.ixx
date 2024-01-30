// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <initializer_list>

export module jpt.DynamicArray;

import jpt.Allocator;
import jpt.ToString;
import jpt_private.ContiguousIterator;

export namespace jpt
{
	/** A sequence container that encapsulates dynamic size arrays. */
	template<typename _TData, class _TAllocator = Allocator<_TData>>
	class DynamicArray
	{
	public:

	};
}