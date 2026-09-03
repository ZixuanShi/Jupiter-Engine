// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_TwoSum;

import jpt.TestFramework;
import jpt.TypeDefs;
import std;

/** Indices of the two entries summing to target, or empty when there is no such pair.
    One pass: each element looks for the complement an earlier element already recorded. */
std::vector<int32> TwoSum(const std::vector<int32>& nums, int32 target)
{
    std::unordered_map<int32, int32> seen;

    for (int32 i = 0; i < static_cast<int32>(nums.size()); ++i)
    {
        if (const auto it = seen.find(target - nums[i]); it != seen.end())
        {
            return { it->second, i };
        }

        seen[nums[i]] = i;
    }

    return {};
}

export void RunUnitTests_TwoSum(jpt::TestCase& test)
{
    test.Expect(TwoSum({ 2, 7, 11, 15 }, 9) == std::vector<int32>{ 0, 1 }, "first two");
    test.Expect(TwoSum({ 3, 2, 4 }, 6) == std::vector<int32>{ 1, 2 }, "not the first element");
    test.Expect(TwoSum({ 3, 3 }, 6) == std::vector<int32>{ 0, 1 }, "duplicate values");
    test.Expect(TwoSum({ 1, 2 }, 99).empty(), "no pair");
}
