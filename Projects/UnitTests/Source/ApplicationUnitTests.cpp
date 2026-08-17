// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "ApplicationUnitTests.h"
#include "Applications/GetApp.h"

import jpt.InputTests;
import jpt.MathTests;
import jpt.TypeDefs;

namespace jpt
{
    bool ApplicationUnitTests::Init()
    {
        if (!ApplicationBase::Init())
        {
            return false;
        }

        RunMathTests();
        RunInputTests();

        // The suites have already run and asserted, so there is nothing left to step.
        SetStatus(Status::Succeeded);

        return true;
    }
}

JPT_SYNC_APP(ApplicationUnitTests)
