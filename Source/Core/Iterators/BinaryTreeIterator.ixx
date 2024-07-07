// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <type_traits>

export module jpt_private.BinaryTreeIterator;

import jpt.TypeDefs;
import jpt.Utilities;

export namespace jpt_private
{
	template<typename TData>
	struct BinaryTreeNode
	{
		TData data;
		BinaryTreeNode* pLeft = nullptr;
		BinaryTreeNode* pRight = nullptr;

		constexpr BinaryTreeNode() = default;
		constexpr BinaryTreeNode(const TData& _data) : data(_data) {}
		constexpr BinaryTreeNode(TData&& _data) : data(jpt::Move(_data)) {}
		constexpr ~BinaryTreeNode();
	};

	template<typename TData>
	constexpr BinaryTreeNode<TData>::~BinaryTreeNode()
	{
		if constexpr (!std::is_trivially_destructible_v<TData>)
		{
			data.~TData();
		}

		pLeft = nullptr;
		pRight = nullptr;
	}
}