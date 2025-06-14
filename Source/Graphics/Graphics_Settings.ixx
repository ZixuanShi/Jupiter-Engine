// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Graphics_Settings;

import jpt.Graphics_Enums;
import jpt.TypeDefs;

export namespace jpt
{
    class GraphicsSettings
    {
    private:
        TimePrecision m_targetFPS = -1.0f;
        VSyncMode m_VSyncMode     = VSyncMode::On;

    public:
        bool PreInit();

    public:
        bool ShouldCapFPS() const { return m_targetFPS > 0.0 && m_VSyncMode == VSyncMode::Off; }
        TimePrecision GetTargetFPS() const { return m_targetFPS; }
        void SetTargetFPS(TimePrecision targetFPS);

        VSyncMode GetVSyncMode() const { return m_VSyncMode; }
        void SetVSyncMode(VSyncMode VSyncMode);
    };
}