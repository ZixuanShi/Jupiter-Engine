// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Graphics.Settings;

import jpt.TypeDefs;

export namespace jpt
{
    class GraphicsSettings
    {
    private:
        TimePrecision m_targetFPS;
        bool m_VSyncOn;

    public:
        bool PreInit();

    public:
        bool ShouldCapFPS() const { return m_targetFPS > 0.0; }
        TimePrecision GetTargetFPS() const { return m_targetFPS; }
        void SetTargetFPS(TimePrecision targetFPS);

        bool GetVSyncOn() const { return m_VSyncOn; }
        void SetVSyncOn(bool vSyncOn);
    };
}