// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.GraphicsSettings;

import jpt.Application;
import jpt.AppSettings;
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
        m_targetFPS = GetSettings(kTargetFPS, -1.0f);
        m_VSyncMode = GetSettings(kVSyncOnKey, VSyncMode::On);

        return true;
    }

    void GraphicsSettings::SetTargetFPS(TimePrecision targetFPS)
    {
        if (AreValuesClose(m_targetFPS, targetFPS))
        {
            return;
        }

        m_targetFPS = targetFPS;
        SetSettings(kTargetFPS, targetFPS, !ShouldCapFPS());
    }

    void GraphicsSettings::SetVSyncMode(VSyncMode VSyncMode)
    {
        if (m_VSyncMode == VSyncMode)
        {
            return;
        }

        m_VSyncMode = VSyncMode;
        GetApplication()->GetRenderer()->RequireReinitSwapChains();
        SetSettings(kVSyncOnKey, VSyncMode, m_VSyncMode == VSyncMode::On);
    }
}