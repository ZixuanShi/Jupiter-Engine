#include "Applications/AppClient.h"

import jpt.LaunchArgs;

namespace jpt
{
    Application& GetApplication()
    {
        static Application s_app;
        return s_app;
    }
}

int main(int argc, char* argv[])
{
    jpt::LaunchArgs::GetInstance().Set(argc, argv);
    jpt::Application& app = jpt::GetApplication();

    if (app.PreInit() && app.Init())
    {
        app.Run();
    }

    app.Terminate();

    return 0;
}
