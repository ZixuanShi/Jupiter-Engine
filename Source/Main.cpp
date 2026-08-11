// Copyright Jupiter Technologies, Inc. All Rights Reserved.

// Must precede SDL_main.h: it makes the header emit an entry point calling the four functions
// below instead of main(). On iOS that is UIApplicationMain, and SDL_AppIterate is the display
// link's animation callback.
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Applications/GetApp.h"

import jpt.TypeDefs;
import jpt.Window;

SDL_AppResult SDL_AppInit(void** ppAppState, int argc, char* argv[])
{
    // jpt::GetApp() is reachable from anywhere, so SDL's appstate would say it twice.
    *ppAppState = nullptr;
    (void)argc; (void)argv;

    jpt::ApplicationBase& app = jpt::GetApp();
    return (app.PreInit() && app.Init()) ? SDL_APP_CONTINUE : SDL_APP_FAILURE;
}

// Unconditionally CONTINUE: nothing but an event ends the app, and returning SUCCESS on a paused
// iOS app would quit it.
SDL_AppResult SDL_AppIterate(void*)
{
    jpt::GetApp().OnFrame();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void*, SDL_Event* pEvent)
{
    switch (jpt::GetApp().GetWindow().OnEvent(*pEvent))
    {
        case jpt::Status::Running: return SDL_APP_CONTINUE;
        case jpt::Status::Failed:  return SDL_APP_FAILURE;
        default:                   return SDL_APP_SUCCESS;
    }
}

// Called exactly once, including when SDL_AppInit reported failure.
void SDL_AppQuit(void*, SDL_AppResult)
{
    jpt::GetApp().Terminate();
}
