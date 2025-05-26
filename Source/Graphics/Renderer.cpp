// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

module jpt.Renderer;

namespace jpt
{
    bool Renderer::PreInit()
    {
        EventManager::GetInstance().Register<Event_Window_Resize>(this, &Renderer::OnWindowResize);
        EventManager::GetInstance().Register<Event_Window_Close>(this, &Renderer::OnWindowClose);

        bool success = true;
        success &= m_settings.PreInit();

        return success;
    }

    bool Renderer::Init()
    {
        if (!m_camera.Init())
        {
            return false;
        }

        return true;
    }

    void Renderer::Update(TimePrecision deltaSeconds)
    {
        m_camera.Update(deltaSeconds);
    }

    const GraphicsSettings& Renderer::GetSettings() const
    {
        return m_settings;
    }
}