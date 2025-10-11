// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.WindowManager;

import jpt.Window;
import jpt.DynamicArray;
import jpt.TypeDefs;

export namespace jpt
{
    class WindowManager
    {
    public:
        static constexpr size_t kMaxWindows = 16;    /**< Maximum number of windows Jupiter can manage simultaneously */

    private:
        DynamicArray<Window*> m_windows;    /**< All the windows managed by Jupiter. index at 0 is the main window */

    public:
        bool PreInit();
        bool Init(const char* mainWindowTitle);

        void Update(TimePrecision deltaSeconds);
        void Terminate();

    public:
        Window* Create(const char* title = Window::kDefaultTitle, int32 width = Window::kDefaultWidth, int32 height = Window::kDefaultHeight);
        void Destroy(const Window* pWindowToDestroy);

    public:
        Window* GetMainWindow();
    };
}