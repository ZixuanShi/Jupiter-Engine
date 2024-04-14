// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Macros.h"
#include "Debugging/Assert.h"

export module jpt.SortedMap;

import jpt.Concepts;
import jpt.Constants;
import jpt.Pair;
import jpt.Utilities;
import jpt.Math;

import jpt_private.BinaryTreeIterator;

export namespace jpt
{
	/** Sorted key value paired structure. Red Black Tree under the hood */
	template<typename _TKey, typename _TValue>
	class SortedMap
	{
	public:
		using TKey   = _TKey;
		using TValue = _TValue;
		using TData  = Pair<TKey, TValue>;
		using TNode  = jpt_private::BinaryTreeNode<TData>;

	private:
		TNode* m_pRoot = nullptr;
		size_t m_size  = 0;

	public:
		constexpr SortedMap() = default;
	};
}