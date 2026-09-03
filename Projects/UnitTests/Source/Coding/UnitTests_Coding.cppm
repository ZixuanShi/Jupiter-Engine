// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_Coding;

import jpt.TestFramework;

import UnitTests_DistanceTree;
import UnitTests_MaxSubArray;
import UnitTests_QuadTree;
import UnitTests_RangesQuery;
import UnitTests_Scratch;
import UnitTests_SetBitsInRange;
import UnitTests_TwoSum;

export void RunUnitTests_Coding()
{
    jpt::TestCase::Run("Coding.TwoSum",         &RunUnitTests_TwoSum);
    jpt::TestCase::Run("Coding.MaxSubArray",    &RunUnitTests_MaxSubArray);
    jpt::TestCase::Run("Coding.SetBitsInRange", &RunUnitTests_SetBitsInRange);
    jpt::TestCase::Run("Coding.RangesQuery",    &RunUnitTests_RangesQuery);
    jpt::TestCase::Run("Coding.DistanceTree",   &RunUnitTests_DistanceTree);
    jpt::TestCase::Run("Coding.QuadTree",       &RunUnitTests_QuadTree);

    // Last, so whatever is being scratched on lands at the bottom of the run.
    jpt::TestCase::Run("Coding.Scratch", &RunUnitTests_Scratch);
}
