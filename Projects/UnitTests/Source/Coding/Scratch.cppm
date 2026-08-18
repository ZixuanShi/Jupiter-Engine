// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Coding.Scratch;

import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Logger;
import std;

namespace jpt::Coding
{
    void Test(TestCase& test)
    {
        test.Expect(true, "placeholder");
    }

    static TestCase s_scratch("Coding.Scratch", &Test);
}