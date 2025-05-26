// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

module jpt.Graphics.Settings;

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
}