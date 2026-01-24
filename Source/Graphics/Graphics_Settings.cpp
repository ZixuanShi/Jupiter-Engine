// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.Graphics_Settings;

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

    bool Graphics_Settings::PreInit()
    {
        m_targetFPS = GetSettings(kTargetFPS, -1.0f);
        m_VSyncMode = GetSettings(kVSyncOnKey, VSyncMode::On);

        return true;
    }

    void Graphics_Settings::SetTargetFPS(TimePrecision targetFPS)
    {
        if (AreValuesClose(m_targetFPS, targetFPS))
        {
            return;
        }

        m_targetFPS = targetFPS;
        SetSettings(kTargetFPS, targetFPS, !ShouldCapFPS());
    }

    void Graphics_Settings::SetVSyncMode(VSyncMode VSyncMode)
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