// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <type_traits>

export module jpt_private.RedBlackTreeIterator;

import jpt.TypeDefs;
import jpt.Utilities;

export namespace jpt_private
{
	template<typename TData>
	struct RedBlackTreeNode
	{
		enum class EColor : uint8
		{
			Undefined,
			Red,
			Black
		};

		TData data;
		RedBlackTreeNode* pParent = nullptr;
		RedBlackTreeNode* pLeftChild = nullptr;
		RedBlackTreeNode* pRightChild = nullptr;
		EColor color = EColor::Undefined;

		constexpr RedBlackTreeNode(const TData& _data) : data(_data) {}
		constexpr RedBlackTreeNode(TData&& _data) : data(jpt::Move(_data)) {}

		constexpr ~RedBlackTreeNode();
	};

	template<typename TData>
	constexpr RedBlackTreeNode<TData>::~RedBlackTreeNode()
	{
		if constexpr (!std::is_trivially_destructible_v<TData>)
		{
			data.~TData();
		}

		pParent = nullptr;
		pLeftChild = nullptr;
		pRightChild = nullptr;
		color = EColor::Undefined;
	}
}

