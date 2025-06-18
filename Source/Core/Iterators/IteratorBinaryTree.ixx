// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.BinaryTreeIterator;

import jpt.TypeDefs;
import jpt.TypeTraits;
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
        if constexpr (!jpt::IsTriviallyDestructible<TData>)
        {
            data.~TData();
        }

        pLeft = nullptr;
        pRight = nullptr;
    }
}