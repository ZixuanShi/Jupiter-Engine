// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Applications/GetApp.h"

import jpt.LaunchArgs;

int main(int argc, char* argv[])
{
    jpt::LaunchArgs::GetInstance().Set(argc, argv);
    jpt::ApplicationBase& app = jpt::GetApp();

    if (app.PreInit() && app.Init())
    {
        app.Run();
    }

    app.Terminate();

    return 0;
}
