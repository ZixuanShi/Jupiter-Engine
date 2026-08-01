// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Application;

export namespace jpt
{
    /** Base class every executable (Editor, and each Projects/<Name> App) derives from. */
    class Application
    {
    public:
        virtual ~Application() = default;

        virtual bool PreInit() { return true; }
        virtual bool Init()    { return true; }
        virtual void Update()  {}
        virtual void Terminate() {}
    };
}
