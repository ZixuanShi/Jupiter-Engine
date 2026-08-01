// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

module jpt.Application;

import jpt.Logger;

namespace jpt
{
    bool Application::PreInit()
    {
        return true;
    }

    bool Application::Init()
    {
        m_status = Status::Running;
        return true;
    }

    void Application::Update(float64 deltaSeconds)
    {
        static float64 s_totalTime = 0.0;
        s_totalTime += deltaSeconds;
        if (s_totalTime > 5.0)
        {
            m_status = Status::Succeeded;
            Debug("Application::Update() - 5 seconds elapsed, exiting.");
        }
    }

    void Application::Terminate()
    {

    }

    void Application::Run()
    {
        while (m_status == Status::Running)
        {
            m_frameTimer.BeginFrame();

            Update(m_frameTimer.GetDeltaSeconds());

            m_frameTimer.EndFrame();
        }
    }
}