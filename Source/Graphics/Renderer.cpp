// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Logger.h"
#include "Core/Minimal/Utilities.h"

module jpt.Renderer;

import jpt.Application;

namespace jpt
{
    bool Renderer::PreInit()
    {
        EventManager::GetInstance().Register<Event_Window_Resize>(this, &Renderer::OnWindowResize);
        EventManager::GetInstance().Register<Event_Window_Close>(this, &Renderer::OnWindowClose);

        JPT_ENSURE(m_api != GraphicsAPI::Unknown);
        JPT_ENSURE(m_settings.PreInit());

        return true;
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

    Graphics_Settings& Renderer::GetSettings()
    {
        return m_settings;
    }

    const Graphics_Settings& Renderer::GetSettings() const
    {
        return m_settings;
    }
}