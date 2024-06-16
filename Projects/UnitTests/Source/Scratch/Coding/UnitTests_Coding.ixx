// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

export module UnitTests_Coding;

import jpt.CoreModules;

int SubArraySum(const jpt::DynamicArray<int32>& nums, int k)
{
	jpt::HashMap<int32, int32> prefixSum;
	prefixSum[0] = 1;

	int sum = 0;
	int count = 0;

	for (int32 i : nums)
	{
		sum += i;

		if (prefixSum.Contains(sum - k))
		{
			count += prefixSum[sum - k];
		}

		++prefixSum[sum];
	}

	return count;
}

struct TreeNode 
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

int FindZigZag(TreeNode* pNode, bool goRight, int depth)
{
	if (pNode == nullptr)
	{
		return 0;
	}

	if (goRight)
	{
		return std::max(FindZigZag(pNode->right, false, depth + 1), FindZigZag(pNode->left, true, depth + 1)) + 1;
	}
	else
	{
		return std::max(FindZigZag(pNode->left, true, depth + 1), FindZigZag(pNode->right, false, depth + 1)) + 1;
	}
}

int longestZigZag(TreeNode* root)
{
	return std::max(FindZigZag(root->left, true, 1), FindZigZag(root->right, false, 1));
}

export bool RunUnitTests_Coding()
{
	return true;
}