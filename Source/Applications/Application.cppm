// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt.Application;

import jpt.TypeDefs;
import jpt.FrameTimer;

export namespace jpt
{
    /** Base class every executable (Editor, and each Projects/<Name> App) derives from. */
    class Application
    {
    private:
        FrameTimer m_frameTimer;
        Status m_status = Status::Pending;

    public:
        virtual ~Application() = default;

        virtual bool PreInit();
        virtual bool Init();
        virtual void Update(float64 deltaSeconds);
        virtual void Terminate();
    
        void Run();
    };
}
