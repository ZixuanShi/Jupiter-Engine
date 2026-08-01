import jpt.Application;

int main() 
{
    jpt::Application app;
    if (app.PreInit() && app.Init())
    {
        app.Run();
    }

    app.Terminate();

    return 0;
}
