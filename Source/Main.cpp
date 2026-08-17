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

namespace local
{
    /** The app's status is the whole lifecycle; SDL only ever asks whether to keep going. */
    SDL_AppResult ToAppResult(jpt::Status status)
    {
        switch (status)
        {
            case jpt::Status::Pending:
            case jpt::Status::Running:
            case jpt::Status::Paused:
                return SDL_APP_CONTINUE;

            case jpt::Status::Failed:
                return SDL_APP_FAILURE;

            default:
                return SDL_APP_SUCCESS;
        }
    }
}

SDL_AppResult SDL_AppInit(void** ppAppState, int argc, char* argv[])
{
    // jpt::GetApp() is reachable from anywhere, so SDL's appstate would say it twice.
    *ppAppState = nullptr;

    jpt::ApplicationBase& app = jpt::GetApp();
    if (app.PreInit(argc, argv) && app.Init())
    {
        return SDL_APP_CONTINUE;
    }
    else
    {
        return SDL_APP_FAILURE;
    }
}

SDL_AppResult SDL_AppIterate(void*)
{
    jpt::ApplicationBase& app = jpt::GetApp();
    app.OnFrame();
    return local::ToAppResult(app.GetStatus());
}

SDL_AppResult SDL_AppEvent(void*, SDL_Event* pEvent)
{
    jpt::ApplicationBase& app = jpt::GetApp();
    app.GetWindow().OnEvent(*pEvent);
    return local::ToAppResult(app.GetStatus());
}

// Called exactly once, including when SDL_AppInit reported failure.
void SDL_AppQuit(void*, SDL_AppResult)
{
    jpt::GetApp().Terminate();
}
