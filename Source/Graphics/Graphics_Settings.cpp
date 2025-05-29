// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

module jpt.Graphics.Settings;

import jpt.Application;
import jpt.Renderer;

import jpt.ProjectSettings;

import jpt.String;
import jpt.Math;

namespace jpt
{
    static const String kTargetFPS  = "targetFPS";
    static const String kVSyncOnKey = "VSyncOn";

    bool GraphicsSettings::PreInit()
    {
        m_targetFPS = ProjectSettings::GetInstance().Get(kTargetFPS, -1.0f);
        m_VSyncOn   = ProjectSettings::GetInstance().Get(kVSyncOnKey, true);

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