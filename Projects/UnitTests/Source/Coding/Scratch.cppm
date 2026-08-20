// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Coding.Scratch;

import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Logger;
import std;

namespace jpt::Coding
{
    void Test(TestCase&)
    {
    }

    // Named to sort last, so scratch output lands at the bottom of the run.
    static TestCase s_scratch("Scratch.Scratch", &Test);
}
