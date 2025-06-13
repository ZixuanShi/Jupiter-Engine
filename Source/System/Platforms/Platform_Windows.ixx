// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Platform.Windows;

#if IS_PLATFORM_WINDOWS

import jpt.Platform;
import jpt.TypeDefs;
import jpt.Utilities;

using HINSTANCE = struct HINSTANCE__*;

export namespace jpt
{
    class Platform_Windows : public Platform
    {
    private:
        using Super = Platform;

    private:
        /** The handle to an instance or handle to a module.
            The operating system uses this value to identify the executable or EXE when it's loaded in memory.
            Certain Windows functions need the instance handle, for example to load icons or bitmaps. */
        HINSTANCE m_hInstance = nullptr;

        /** A flag that indicates whether the main application window is minimized, maximized, or shown normally. */
        int32 m_nCmdShow = 0;

    public:
        Platform_Windows() = default;
        Platform_Windows(HINSTANCE hInstance, int nCmdShow);

        virtual bool PreInit() override;

    public:
        HINSTANCE GetHINSTANCE() const { return m_hInstance; }
        int32 GetnCmdShow() const { return m_nCmdShow; }
    };
}

#endif // IS_PLATFORM_WINDOWS
