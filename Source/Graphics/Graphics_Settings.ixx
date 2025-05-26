// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Graphics.Settings;

export namespace jpt
{
    struct GraphicsSettings
    {
        bool VSyncOn;

    public:
        bool PreInit();
    };
}