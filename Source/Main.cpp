#include "Applications/AppClient.h"

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
    jpt::Application& app = jpt::GetApplication();

    if (app.PreInit() && app.Init())
    {
        app.Run(argc, argv);
    }

    app.Terminate();

    return 0;
}
