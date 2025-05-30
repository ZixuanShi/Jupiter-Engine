// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.Graphics.Settings;

import jpt.Application;
import jpt.Renderer;

import jpt.ProjectSettings;

import jpt.String;
import jpt.ToString;
import jpt.Math;

namespace jpt
{
    static const String kTargetFPS  = "targetFPS";
    static const String kVSyncOnKey = "VSyncMode";

    bool GraphicsSettings::PreInit()
    {
        const ProjectSettings& projSettings = ProjectSettings::GetInstance();

        m_targetFPS = projSettings.Get(kTargetFPS, -1.0f);
        m_VSyncMode = static_cast<VSyncMode>(projSettings.Get(kVSyncOnKey, static_cast<int32>(VSyncMode::On)));
        
        return true;
    }

    void GraphicsSettings::SetTargetFPS(TimePrecision targetFPS)
    {
        if (AreValuesClose(m_targetFPS, targetFPS))
        {
            return;
        }

        m_targetFPS = targetFPS;

        if (!ShouldCapFPS())
        {
            ProjectSettings::GetInstance().Erase(kTargetFPS);
        }
        else
        {
            ProjectSettings::GetInstance().Set(kTargetFPS, targetFPS);
        }
    }

    void GraphicsSettings::SetVSyncMode(VSyncMode VSyncMode)
    {
        if (m_VSyncMode == VSyncMode)
        {
            return;
        }

        m_VSyncMode = VSyncMode;

        GetApplication()->GetRenderer()->SetShouldRecreateSwapChains();

        if (m_VSyncMode == VSyncMode::On)
        {
            ProjectSettings::GetInstance().Erase(kVSyncOnKey);
        }
        else
        {
            ProjectSettings::GetInstance().Set(kVSyncOnKey, static_cast<int32>(m_VSyncMode));
        }
    }
}