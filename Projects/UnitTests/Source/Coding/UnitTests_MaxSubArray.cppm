// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_MaxSubArray;

import jpt.TestFramework;
import jpt.TypeDefs;
import std;

/** Largest sum of any contiguous subarray. Kadane's: a running sum that restarts whenever
    carrying it forward is worse than starting over. */
int32 MaxSubArray(const std::vector<int32>& nums)
{
    int32 maxSum = nums[0];
    int32 currentSum = nums[0];

    for (usize i = 1; i < nums.size(); ++i)
    {
        currentSum = std::max(nums[i], currentSum + nums[i]);
        maxSum = std::max(maxSum, currentSum);
    }

    return maxSum;
}

/** Kadane's again, with the restart made explicit so the winning run's indices can be
    recorded. Ties keep the first maximal subarray. */
struct MaxSubArrayResult
{
    int32 sum;
    usize begin;    // first index of the winning run
    usize end;      // last index, inclusive

    bool operator==(const MaxSubArrayResult&) const = default;
};

MaxSubArrayResult MaxSubArrayRange(const std::vector<int32>& nums)
{
    MaxSubArrayResult result{ nums[0], 0, 0 };

    int32 currentSum = nums[0];
    usize currentBegin = 0;

    for (usize i = 1; i < nums.size(); ++i)
    {
        if (currentSum < 0)     // carrying it forward is worse — the max below restarts at i
        {
            currentBegin = i;
        }
        currentSum = std::max(nums[i], currentSum + nums[i]);

        if (currentSum > result.sum)
        {
            result.sum = currentSum;
            result.begin = currentBegin;
            result.end = i;
        }
    }

    return result;
}

export void RunUnitTests_MaxSubArray(jpt::TestCase& test)
{
    test.Expect(MaxSubArray({ -2, 1, -3, 4, -1, 2, 1, -5, 4 }) == 6, "mixed signs");
    test.Expect(MaxSubArray({ 1 }) == 1, "single element");
    test.Expect(MaxSubArray({ 5, 4, -1, 7, 8 }) == 23, "all positive");
    test.Expect(MaxSubArray({ -3, -1, -7 }) == -1, "all negative takes the largest");

    test.Expect(MaxSubArrayRange({ -2, 1, -3, 4, -1, 2, 1, -5, 4 }) == MaxSubArrayResult{ 6, 3, 6 }, "mixed signs range");
    test.Expect(MaxSubArrayRange({ 1 }) == MaxSubArrayResult{ 1, 0, 0 }, "single element range");
    test.Expect(MaxSubArrayRange({ 5, 4, -1, 7, 8 }) == MaxSubArrayResult{ 23, 0, 4 }, "all positive range");
    test.Expect(MaxSubArrayRange({ -3, -1, -7 }) == MaxSubArrayResult{ -1, 1, 1 }, "all negative range");
    test.Expect(MaxSubArrayRange({ 3, -3, 3 }) == MaxSubArrayResult{ 3, 0, 0 }, "tie keeps the first run");
}
