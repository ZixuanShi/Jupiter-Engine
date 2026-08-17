// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Coding.MaxSubArray;

import jpt.TestFramework;
import jpt.TypeDefs;
import std;

namespace jpt::Coding
{
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

    void Test(TestCase& test)
    {
        test.Expect(MaxSubArray({ -2, 1, -3, 4, -1, 2, 1, -5, 4 }) == 6, "mixed signs");
        test.Expect(MaxSubArray({ 1 }) == 1, "single element");
        test.Expect(MaxSubArray({ 5, 4, -1, 7, 8 }) == 23, "all positive");
        test.Expect(MaxSubArray({ -3, -1, -7 }) == -1, "all negative takes the largest");
    }

    static TestCase s_maxSubArray("Coding.MaxSubArray", &Test);
}
