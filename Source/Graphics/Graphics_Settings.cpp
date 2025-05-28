// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

module jpt.Graphics.Settings;

import jpt.Application;
import jpt.Renderer;

import jpt.ProjectSettings;

import jpt.String;

namespace jpt
{
    static const String kVSyncOnKey = "VSyncOn";

    bool GraphicsSettings::PreInit()
    {
        m_VSyncOn = ProjectSettings::GetInstance().Get(kVSyncOnKey, true);

        return true;
    }

    bool GraphicsSettings::GetVSyncOn() const
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
            ProjectSettings::GetInstance().Erase(kVSyncOnKey);
        }
        else
        {
            ProjectSettings::GetInstance().Set(kVSyncOnKey, false);
        }
    }
}