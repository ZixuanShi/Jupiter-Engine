// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests.Coding;

import UnitTests.TestFramework;

import UnitTests.FindFiles;
import UnitTests.CharShift;
import UnitTests.DistanceTree;
import UnitTests.QuadTree;
import UnitTests.RangesQuery;
import UnitTests.Scratch;
import UnitTests.SetBitsInRange;
import UnitTests.GridFindWords;

export void RunUnitTests_Coding()
{
    // Boogle. Find all possible words out of a grid with rules

    jpt::TestCase::Run("Coding.CharShift",      &RunUnitTests_CharShift);
    jpt::TestCase::Run("Coding.SetBitsInRange", &RunUnitTests_SetBitsInRange);
    jpt::TestCase::Run("Coding.RangesQuery",    &RunUnitTests_RangesQuery);
    jpt::TestCase::Run("Coding.DistanceTree",   &RunUnitTests_DistanceTree);
    jpt::TestCase::Run("Coding.QuadTree",       &RunUnitTests_QuadTree);

    // Last, so whatever is being scratched on lands at the bottom of the run.
    jpt::TestCase::Run("Coding.Scratch", &RunUnitTests_Scratch);

    jpt::TestCase::Run("Coding.FindFiles",      &RunUnitTests_FindFiles);
    jpt::TestCase::Run("Coding.GridFindWords",  &RunUnitTests_GridFindWords);
}
