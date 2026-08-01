import jpt.Application;
import jpt.Logger;
import jpt.Utils;

int main() 
{
    jpt::Debug("Jupiter Engine from {}-{}", jpt::GetPlatformName(), jpt::GetConfigName());

    jpt::Application app;
    if (app.PreInit() && app.Init())
    {
        app.Run();
    }

    app.Terminate();

    return 0;
}
