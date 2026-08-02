import jpt.Application;

int main(int argc, char* argv[])
{
    jpt::Application app;
    if (app.PreInit() && app.Init())
    {
        app.Run(argc, argv);
    }

    app.Terminate();

    return 0;
}
