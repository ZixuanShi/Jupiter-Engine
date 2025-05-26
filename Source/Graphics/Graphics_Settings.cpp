// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

module jpt.Graphics.Settings;

import jpt.Application;
import jpt.Renderer;

import jpt.ProjectSettings;

namespace jpt
{
    bool GraphicsSettings::PreInit()
    {
        m_VSyncOn = ProjectSettings::GetInstance().Get("VSyncOn", true);

        return true;
    }

    bool GraphicsSettings::IsVSyncOn() const
    {
        return m_VSyncOn;
    }

    void GraphicsSettings::SetVSyncOn(bool vSyncOn)
    {
        if (m_VSyncOn == vSyncOn)
        {
            return;
        }

        m_VSyncOn = vSyncOn;

        GetApplication()->GetRenderer()->SetShouldRecreateSwapChains();

        if (vSyncOn)
        {
            ProjectSettings::GetInstance().Erase("VSyncOn");
        }
        else
        {
            ProjectSettings::GetInstance().Set("VSyncOn", false);
        }
    }
}