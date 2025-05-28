// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Graphics.Settings;

export namespace jpt
{
    class GraphicsSettings
    {
    private:
        bool m_VSyncOn;

    public:
        bool PreInit();

    public:
        bool GetVSyncOn() const;
        void SetVSyncOn(bool vSyncOn);
    };
}