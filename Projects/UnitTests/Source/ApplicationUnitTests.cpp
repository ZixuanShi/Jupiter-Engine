// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// Before the headers that import, so SDL's declarations are not redefinitions of a module's.
#include <SDL3/SDL.h>

#include "ApplicationUnitTests.h"
#include "Applications/GetApp.h"

import jpt.InputTests;
import jpt.MathTests;

namespace jpt
{
    bool ApplicationUnitTests::PreInit(int argc, char* argv[])
    {
        if (!ApplicationBase::PreInit(argc, argv))
        {
            return false;
        }

        RunMathTests();

        // After the base pre-initialised Input, which the suite asserts against. Still before
        // Init() opens a window, which it deliberately does not need.
        RunInputTests();

        return true;
    }

    bool ApplicationUnitTests::Init()
    {
        if (!ApplicationBase::Init())
        {
            return false;
        }

        // Nothing but an event ends a run -- SDL_AppIterate always continues -- so the verdict has
        // to leave through one. Window::OnEvent maps this to Status::Succeeded, hence exit 0;
        // returning false above is the other half, and exits 1.
        SDL_Event quit = {};
        quit.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&quit);

        return true;
    }
}

JPT_SYNC_APP(ApplicationUnitTests)
