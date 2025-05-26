// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

module jpt.Graphics.Settings;

import jpt.ProjectSettings;

namespace jpt
{
    bool GraphicsSettings::PreInit()
    {
        VSyncOn = ProjectSettings::GetInstance().Get("VSyncOn", true);

        return true;
    }
}