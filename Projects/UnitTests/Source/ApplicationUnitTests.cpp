// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "ApplicationUnitTests.h"
#include "Applications/GetApp.h"

import jpt.TestFramework;
import jpt.TypeDefs;

namespace jpt
{
    bool ApplicationUnitTests::Init()
    {
        if (!ApplicationBase::Init())
        {
            return false;
        }

        // Every suite registered itself during static initialisation, so nothing here names one.
        // RunAll's verdict is the run's: Main.cpp maps Status::Failed to SDL_APP_FAILURE, hence
        // exit 1, and the run is over either way -- there is nothing left to step.
        SetStatus(TestCase::RunAll());

        return true;
    }
}

JPT_SYNC_APP(ApplicationUnitTests)
