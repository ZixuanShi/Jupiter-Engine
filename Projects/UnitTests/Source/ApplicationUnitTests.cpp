// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "ApplicationUnitTests.h"
#include "Applications/GetApp.h"

import jpt.TestFramework;
import jpt.TypeDefs;

import UnitTests_Coding;
import UnitTests_Core;
import UnitTests_Input;
import UnitTests_Scene;

namespace jpt
{
    bool ApplicationUnitTests::Init()
    {
        if (!ApplicationBase::Init())
        {
            return false;
        }

        // The whole run, in the order it happens. Each of these is itself a list of suites, and
        // each suite a list of cases -- so every test is reachable by reading downwards from here.
        RunUnitTests_Coding();
        RunUnitTests_Core();
        RunUnitTests_Input();
        RunUnitTests_Scene();

        // Summarize's verdict is the run's: Main.cpp maps Status::Failed to SDL_APP_FAILURE, hence
        // exit 1, and the run is over either way -- there is nothing left to step.
        SetStatus(TestCase::Summarize());

        return true;
    }
}

JPT_SYNC_APP(ApplicationUnitTests)
